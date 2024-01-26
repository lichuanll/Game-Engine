#pragma once
#include"Core.h"

#include"Window.h"
#include"Events/Event.h"
#include"Hazel/Events/ApplicationEvent.h"
#include"Hazel/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

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

		//unsigned int m_VertexArray;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<Shader> m_Shader2;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

		std::shared_ptr<VertexArray> m_SquareVA;
	};

	Application* CreateApplication();
}
