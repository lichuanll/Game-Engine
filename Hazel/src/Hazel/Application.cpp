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
		WindowResizeEvent e(1280, 720);//��������¼���ʵ��
		HZ_TRACE(e);//ͨ���˺��ӡ�������������ع�������������Կ�������д
		while (true)
		{

		}
	}
}