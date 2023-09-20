#include"hzpch.h"
#include"Application.h"
#include"Hazel/Events/ApplicationEvent.h"
#include"Hazel/Log.h"
#include<GLFW/glfw3.h>
namespace Hazel
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window> (Window::Create());
	}
	Application::~Application()
	{

	}
	void Application::Run()
	{
		
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
		WindowResizeEvent e(1280, 720);//创建这个事件的实例
		HZ_TRACE(e);//通过此宏打印出来，由于重载过右移运算符所以可以这样写
	}
}