#include "hzpch.h"
#include "CameraController.h"

#include "KeyCodes.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Hazel/Renderer/PerspectiveCamera.h"

namespace Hazel
{
	CameraController::CameraController(float aspectRatio,bool rotation)
		: m_AspectRatio(aspectRatio),m_Rotation(rotation)
	{
		switch (m_Camera->GetCameraType())
		{
		case Camera::CameraType::Orthographic:
			cameraType = m_Camera->GetCameraType();
			m_Camera = std::make_shared<OrthographicCamera>(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
			break;
		case Camera::CameraType::Perspective:
			cameraType = m_Camera->GetCameraType();
			m_Camera = std::make_shared<PerspectiveCamera>(45.0f, aspectRatio);
			break;
		}
	}

	void CameraController::OnUpdate(Timestep ts)
	{
		if(cameraType == Camera::CameraType::Orthographic)
		{
			OrthographicCameraOnUpdate(ts);
		}
		else if(cameraType == Camera::CameraType::Perspective)
		{
			PerspectiveCameraOnUpdate(ts);
		}
	}

	void CameraController::OnEvent(Event& e)
	{
		if (cameraType == Camera::CameraType::Orthographic)
		{
			OrthographicCameraOnEvent(e);
		}
		else if (cameraType == Camera::CameraType::Perspective)
		{
			PerspectiveCameraOnEvent(e);
		}
	}

	void CameraController::ChangeCameraType(Camera::CameraType type)
	{
		Camera::SetCameraType(type);
		cameraType = type;
		if(type == Camera::CameraType::Orthographic)
		{
			m_Camera = std::make_shared<OrthographicCamera>(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		}
		else if(type == Camera::CameraType::Perspective)
		{
			m_Camera = std::make_shared<PerspectiveCamera>(45.0f, m_AspectRatio);
		}
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f ;//缩放级别
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		glm::mat4 ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
		m_Camera->SetProjection(ProjectionMatrix);
		
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		glm::mat4 ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
		m_Camera->SetProjection(ProjectionMatrix);
		return false;
	}

	bool CameraController::OnMouseMove(MouseMovedEvent& e)
	{
		
		return false;
	}

	void CameraController::OrthographicCameraOnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}
		else if (Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}
		if (Input::IsKeyPressed(HZ_KEY_W))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		else if (Input::IsKeyPressed(HZ_KEY_S))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}
		if (m_Rotation)
		{
			if (Input::IsKeyPressed(HZ_KEY_Q))
			{
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}
			if (Input::IsKeyPressed(HZ_KEY_E))
			{
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			m_Camera->SetRotation(m_CameraRotation);

		}
		m_Camera->SetPosition(m_CameraPosition);
		m_CameraMoveSpeed = m_ZoomLevel;
	}

	void CameraController::OrthographicCameraOnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(CameraController::OnWindowResized));
	}

	void CameraController::PerspectiveCameraOnUpdate(Timestep ts)
	{
		
		if(Input::IsMouseButtonPressed(1))//右键按住，中键为2，左键为0//Input::IsMouseButtonPressed(1)
		{
			float _xpos = Input::GetMouseX();
			float _ypos = Input::GetMouseY();
			if (m_firstMove)
			{
				m_xpos = _xpos;
				m_ypos = _ypos;
				m_firstMove = false;
				return;
			}
			float _xOffset = _xpos - m_xpos;
			float _yOffset = -(_ypos - m_ypos);

			m_xpos = _xpos;//更新上一次的数值
			m_ypos = _ypos;//更新上一次的数值

			m_pitch += _yOffset * m_sensitivity;
			if (m_pitch >= 89.0f)
			{
				m_pitch = 89.0f;
			}
			if (m_pitch <= -89.0f)
			{
				m_pitch = -89.0f;
			}

			m_front.y = sin(glm::radians(m_pitch));
			m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
			m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

			m_front = normalize(m_front);

			m_yaw += _xOffset * m_sensitivity;

			m_front.y = sin(glm::radians(m_pitch));
			m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
			m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

			m_front = normalize(m_front);
			if (Input::IsKeyPressed(HZ_KEY_W))
			{
				m_CameraPosition += m_front * m_CameraMoveSpeed * ts.GetSeconds();
			}
			else if (Input::IsKeyPressed(HZ_KEY_S))
			{
				m_CameraPosition -= m_front * m_CameraMoveSpeed * ts.GetSeconds();
			}
			if (Input::IsKeyPressed(HZ_KEY_A))
			{
				m_CameraPosition -= normalize(cross(m_front, m_up)) * m_CameraMoveSpeed * ts.GetSeconds();
			}
			else if (Input::IsKeyPressed(HZ_KEY_D))
			{
				m_CameraPosition += normalize(cross(m_front, m_up)) * m_CameraMoveSpeed * ts.GetSeconds();
			}
			//m_firstMove = true;
			
		}
		m_Camera->SetPerspectiveRotation(m_CameraPosition, m_front, m_up);
		
	}

	void CameraController::PerspectiveCameraOnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(HZ_BIND_EVENT_FN(CameraController::OnMouseMove));
	}
}
