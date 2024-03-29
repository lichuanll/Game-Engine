#pragma once
#include"Core.h"

#include"Window.h"
#include"Hazel/Events/Event.h"
#include"Hazel/Events/ApplicationEvent.h"
#include"Hazel/Core/LayerStack.h"

#include "Hazel/Core/Timestep.h"

#include "Hazel/ImGui/ImGuiLayer.h"



namespace Hazel
{
	class HAZEL_API Application
	{
	public:
		
		Application();
		virtual ~Application();
		void Run();
		void StopRun();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);
		inline static  Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		static Application* s_Instance;
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		Timestep m_Timestep;
		float m_LastFrameTime;
		//unsigned int m_VertexArray;
		
	};

	Application* CreateApplication();
}
