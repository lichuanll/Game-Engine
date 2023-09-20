#pragma once
#include"Core.h"

#include"Window.h"
#include"Events/Event.h"
#include"Hazel/Events/ApplicationEvent.h"
#include"Hazel/LayerStack.h"

namespace Hazel
{
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();
}