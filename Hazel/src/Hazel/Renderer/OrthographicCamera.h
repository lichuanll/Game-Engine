#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
namespace Hazel
{
	class OrthographicCamera :public Camera
	{
	public:
		
		OrthographicCamera(float left, float right, float bottom, float top);

		virtual void SetProjection(glm::mat4 ProjectionMatrix) override;

		virtual const glm::vec3& GetPosition()const override { return m_Position; }
		virtual void SetPosition(const glm::vec3& position) override { m_Position = position; RecalculateViewMatrix(); }
		
		virtual float GetRotation()const override { return m_Rotation; }
		virtual void SetRotation(float rotation) override { m_Rotation = rotation; RecalculateViewMatrix(); }
		virtual void SetPerspectiveRotation(glm::vec3 pos, glm::vec3 front, glm::vec3 up) override;
		virtual const glm::mat4& GetProjectionMatrix()const override { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix()const override { return m_ViewMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix()const override { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::mat4 m_ModelMatrix;

		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		float m_Rotation = 0.0f;
	};
}
