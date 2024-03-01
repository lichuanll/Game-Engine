#include<Hazel.h>
#include "Hazel/Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

class ExampleLayer :public Hazel::Layer 
{
public:
	ExampleLayer() :Layer("Example"),
		 m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),m_CameraPosition(0.0f),m_SquarePosition(0.0f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());



		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));//为VBO分配缓存
		//放入代码块，让这个layout执行完就自动销毁

		Hazel::BufferLayout layout = {
		{Hazel::ShaderDataType::Float3, "a_Position"},
		{Hazel::ShaderDataType::Float4, "a_Color"}

		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		unsigned int indices[3] = { 0,1,2 };
		Hazel::Ref<Hazel::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_SquareVA.reset(Hazel::VertexArray::Create());
		float SquareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));

		squareVB->SetLayout({
			{Hazel::ShaderDataType::Float3, "a_Position" }
			}
		);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int SquareIndices[6] = { 0,1,2,2,3,0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(SquareIndices, sizeof(SquareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color=a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
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
		m_Shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));
		std::string vertexSrc2 = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			
			uniform vec4 u_Color;

			void main()
			{
				color = u_Color;
				
			}
		)";
		m_Shader2.reset(Hazel::Shader::Create(vertexSrc2, fragmentSrc2));
	}
	void OnUpdate(Hazel::Timestep ts)override//加入Time的作用是在循环中相同时间循环不同次数（由于显示器分辨率导致）时，可以获得相同的移动效果
	{
		//HZ_TRACE("Delta time:{0}s ({1}ms)", ts.GetSeconds(),ts.GetMillisecond());
		
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}

		if(Hazel::Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}


		if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
		{
			m_SquarePosition.x -= m_SquareMoveSpeed * ts;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
		{
			m_SquarePosition.x += m_SquareMoveSpeed * ts;
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
		{
			m_SquarePosition.y -= m_SquareMoveSpeed * ts;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
		{
			m_SquarePosition.y += m_SquareMoveSpeed * ts;
		}

		Hazel::RenderCommand::SetClearColor({ 0,0,0,1 });
		Hazel::RenderCommand::Clear();
		Hazel::Renderer::BeginScene(m_Camera);

		glm::mat4 Move = glm::translate(glm::mat4(1.0f), m_SquarePosition);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		//glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		//glm::vec4 BlueColor(0.2f, 0.3f, 0.8f, 1.0f);

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_Shader2)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_Shader2)->UploadUniformFloat4(m_SquareColor,"u_Color");
		for(int y = 0; y < 20; y++)
		{
			for(int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = Move * glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(m_SquareVA, m_Shader2, transform);
			}
		}
		
		


		Hazel::Renderer::Submit(m_VertexArray, m_Shader);


		Hazel::Renderer::EndScene();
	}
	void OnEvent(Hazel::Event& event)override
	{
		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
		
	}
	virtual  void OnImGuiRender() override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
			ImGui::End();
	}
	bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event)
	{
		
		return false;
	}
private:
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::Shader> m_Shader2;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;
	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;

	glm::vec4 m_SquareColor= glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);

};
class Sandbox :public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}

};
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox;
}