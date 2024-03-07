#include"hzpch.h"
#include"Application.h"

#include"Hazel/Core/Log.h"	
#include<GLFW/glfw3.h>

#include "Hazel/Renderer/Renderer.h"


#include "Input.h"
#include <GLFW/glfw3.h>
namespace Hazel
{
#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	
	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window> (Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		Renderer::Init();
		m_ImGuiLayer = new ImGuiLayer();
		
		PushOverLayer(m_ImGuiLayer);
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
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
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
			
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			if(!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)//�����ύҪҪ��Ⱦ���ݵĵط�
					layer->OnUpdate(timestep);
				//auto [x, y] = Input::GetMousePosition();
				//HZ_CORE_TRACE("{0},{1}", x, y);
				
			}
			m_ImGuiLayer->Begin();//����Ҫ��Ⱦ�ĵط�
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		}
		WindowResizeEvent e(1280, 720);//��������¼���ʵ��
		//HZ_TRACE(e);//ͨ���˺��ӡ�������������ع�������������Կ�������д
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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if(e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}
