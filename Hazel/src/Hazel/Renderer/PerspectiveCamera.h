#pragma once
#include "Camera.h"
#include <glm/glm.hpp>
namespace Hazel
{
	class PerspectiveCamera:public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspecet);
		void SetProjection(glm::mat4 ProjectionMatrix) override;

		const glm::vec3& GetPosition()const override { return m_Position; }
		void SetPosition(const glm::vec3& position) override { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation()const override { return 0.0f; }
		virtual void SetRotation(float rotation) override;
		virtual void SetPerspectiveRotation(glm::vec3 pos, glm::vec3 front, glm::vec3 up) override{ m_Position = pos, m_front = front, m_up = up; RecalculateViewMatrix(); }
		const glm::mat4& GetProjectionMatrix()const override { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix()const override { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix()const override { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::mat4 m_ModelMatrix;

		glm::vec3 m_Position = { 0.0f,0.0f,3.0f };
		glm::vec3 m_front, m_up;
	};
}
