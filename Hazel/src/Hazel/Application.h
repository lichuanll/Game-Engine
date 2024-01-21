#pragma once
#include"Core.h"

#include"Window.h"
#include"Events/Event.h"
#include"Hazel/Events/ApplicationEvent.h"
#include"Hazel/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"

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

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		unsigned int m_VertexArray , m_VertexBuffer , m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

	};

	Application* CreateApplication();
}
