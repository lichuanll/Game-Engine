#include "hzpch.h"
#include "OpenGLContext.h"
#include <string>
#include <GLFW/glfw3.h>
#include "glad/glad.h"

namespace Hazel
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
	}

	void OpenGLContext::init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
		HZ_CORE_INFO("OpenGL Info:");
		
		///glGetString返回值 GLubyte类型？
		/*HZ_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		HZ_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		HZ_CORE_INFO("Version: {0}", glGetString(GL_VERSION));*/
	}

	void OpenGLContext::SwapBuffer()
	{

		glfwSwapBuffers(m_WindowHandle);
	}
}
