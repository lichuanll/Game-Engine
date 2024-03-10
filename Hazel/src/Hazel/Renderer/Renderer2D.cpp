#include "hzpch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
//#include "Hazel/Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuardVertexArray;
		Ref<Shader> FlatColorShader;
	};
	static Renderer2DStorage* s_Data;
	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->QuardVertexArray = VertexArray::Create();
		float SquareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));

		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position" }
			}
		);
		s_Data->QuardVertexArray->AddVertexBuffer(squareVB);

		unsigned int SquareIndices[6] = { 0,1,2,2,3,0 };
		Hazel::Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(SquareIndices, sizeof(SquareIndices) / sizeof(uint32_t)));
		s_Data->QuardVertexArray->SetIndexBuffer(squareIB);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::BeginScene(Ref<Camera> m_Camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", m_Camera->GetViewProjectionMatrix());
		
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});
		s_Data->FlatColorShader->SetMat4("u_Transform", transform);
		s_Data->QuardVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuardVertexArray);
	}
}
