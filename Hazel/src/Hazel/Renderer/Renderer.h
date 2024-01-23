#pragma once
namespace Hazel
{
	enum class RenderAPI
	{
		None=0,OpenGL=1
	};
	class Renderer
	{
	public:
		inline static RenderAPI  GetAPI() { return s_RendererAPI; }
	private:
		static RenderAPI s_RendererAPI;
	};
}


