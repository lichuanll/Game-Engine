#include"hzpch.h"
#include"Application.h"

#include"Hazel/Log.h"	
#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include "Input.h"
namespace Hazel
{
#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Hazel::ShaderDataType::Float:    return GL_FLOAT;
		case Hazel::ShaderDataType::Float2:   return GL_FLOAT;
		case Hazel::ShaderDataType::Float3:	  return GL_FLOAT;
		case Hazel::ShaderDataType::Float4:	  return GL_FLOAT;
		case Hazel::ShaderDataType::Mat3:	  return GL_FLOAT;
		case Hazel::ShaderDataType::Mat4:	  return GL_FLOAT;
		case Hazel::ShaderDataType::Int:      return GL_INT;
		case Hazel::ShaderDataType::Int2:     return GL_INT;
		case Hazel::ShaderDataType::Int3:	  return GL_INT;
		case Hazel::ShaderDataType::Int4:	  return GL_INT;
		case Hazel::ShaderDataType::Bool:     return GL_BOOL;
		}
		HZ_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}
	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window> (Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		
		PushOverLayer(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		//放入代码块，让这个layout执行完就自动销毁
		{
			BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}

			};
			m_VertexBuffer->SetLayout(layout);
		}
		
		uint32_t index = 0;
		for(const auto& element : m_VertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				m_VertexBuffer->GetLayout().GetStride(),
				(const void*)element.Offset);
			index++;
		}
		

		

		unsigned int indices[3] = { 0,1,2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color=a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
		m_Shader.reset(new Shader(vertexSrc,fragmentSrc));
	}
	Application::~Application()
	{

	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	
	void Application::OnEvent(Event& e)
	{
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		HZ_CORE_TRACE("{0}", e);
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}
	void Application::Run()
	{
		
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Shader->Bind();

			
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
			m_Shader->UnBind();
			for (Layer* layer : m_LayerStack)//用于提交要要渲染内容的地方
				layer->OnUpdate();
			//auto [x, y] = Input::GetMousePosition();
			//HZ_CORE_TRACE("{0},{1}", x, y);
			m_ImGuiLayer->Begin();//真正要渲染的地方
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
		WindowResizeEvent e(1280, 720);//创建这个事件的实例
		//HZ_TRACE(e);//通过此宏打印出来，由于重载过右移运算符所以可以这样写
	}

	void Application::StopRun()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}