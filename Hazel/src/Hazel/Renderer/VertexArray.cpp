#include "hzpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Hazel/Platform/OpenGL/OpenGLBuffer.h"
#include "Hazel/Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::None:
			HZ_CORE_ASSERT(false, "RendererAPI:None is currently not supported");
			return nullptr;
		case RenderAPI::OpenGL:
			return new OpenGLVertexArray();

		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}
