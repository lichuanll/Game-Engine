#include"hzpch.h"
#include"Application.h"
#include"Hazel/Events/ApplicationEvent.h"
#include"Hazel/Log.h"
namespace Hazel
{
	Application::Application()
	{

	}
	Application::~Application()
	{

	}
	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);//创建这个事件的实例
		HZ_TRACE(e);//通过此宏打印出来，由于重载过右移运算符所以可以这样写
		while (true)
		{

		}
	}
}