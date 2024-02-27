#pragma once
#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Hazel
{
	
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray,const std::shared_ptr<Shader>& shader);
		inline static RendererAPI::API  GetAPI() { return RendererAPI::GetAPI();}
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}


