#include "hzpch.h"
#include "PerspectiveCamera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Hazel
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspecet)
		:m_ProjectionMatrix(glm::perspective(glm::radians(fov),aspecet,0.1f,100.0f))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetProjection(glm::mat4 ProjectionMatrix)
	{
		m_ProjectionMatrix = ProjectionMatrix;
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetRotation(float rotation)
	{
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_front, m_up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
