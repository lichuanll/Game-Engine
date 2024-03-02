#include "hzpch.h"
#include "Renderer.h"

#include "Hazel/Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray,const Ref<Shader>& shader,const glm::mat4& transform )
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader) ->UploadUniformMat4(m_SceneData->ViewProjectionMatrix, "u_ViewProjection");
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4(transform, "u_Transform");
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
