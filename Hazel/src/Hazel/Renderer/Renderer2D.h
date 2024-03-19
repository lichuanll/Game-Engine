#pragma once

#include "Camera.h"
#include "Texture.h"
namespace Hazel
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(Ref<Camera> m_Camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position,const glm::vec2& size,const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture> texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture> texture);

	};
}
