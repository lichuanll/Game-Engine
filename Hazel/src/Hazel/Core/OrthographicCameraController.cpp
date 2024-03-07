#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "KeyCodes.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Hazel/Renderer/PerspectiveCamera.h"

namespace Hazel
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio,bool rotation)
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

	void OrthographicCameraController::OnUpdate(Timestep ts)
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

	void OrthographicCameraController::OnEvent(Event& e)
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

	void OrthographicCameraController::ChangeCameraType(Camera::CameraType type)
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

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f ;//���ż���
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		glm::mat4 ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
		m_Camera->SetProjection(ProjectionMatrix);
		
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		glm::mat4 ProjectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
		m_Camera->SetProjection(ProjectionMatrix);
		return false;
	}

	bool OrthographicCameraController::OnMouseMove(MouseMovedEvent& e)
	{
		float _xpos = e.GetX();
		float _ypos = e.GetY();
		if (m_firstMove)
		{
			m_xpos = _xpos;
			m_ypos = _ypos;
			m_firstMove = false;
			return false;
		}
		float _xOffset = _xpos - m_xpos;
		float _yOffset = -(_ypos - m_ypos);

		m_xpos = _xpos;//������һ�ε���ֵ
		m_ypos = _ypos;//������һ�ε���ֵ

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
		return false;
	}

	void OrthographicCameraController::OrthographicCameraOnUpdate(Timestep ts)
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

	void OrthographicCameraController::OrthographicCameraOnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::PerspectiveCameraOnUpdate(Timestep ts)
	{
		if(1)//�Ҽ���ס���м�Ϊ2�����Ϊ0//Input::IsMouseButtonPressed(1)
		{
			
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
		}
		m_Camera->SetPerspectiveRotation(m_CameraPosition, m_front, m_up);
		
	}

	void OrthographicCameraController::PerspectiveCameraOnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseMove));
	}
}
