#pragma once

#include "Hazel/Renderer/Camera.h"
#include "Timestep.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
namespace Hazel
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		Ref<Camera> GetCamera() { return m_Camera; }
		const Ref<Camera> GetCamera()const { return m_Camera; }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		void ChangeCameraType(Camera::CameraType type);
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel() { return m_ZoomLevel; }
	private:
		void OrthographicCameraOnUpdate(Timestep ts);
		void OrthographicCameraOnEvent(Event& e);
		void PerspectiveCameraOnUpdate(Timestep ts);
		void PerspectiveCameraOnEvent(Event& e);
		bool OnMouseScrolled(MouseScrolledEvent& e); 
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouseMove(MouseMovedEvent& e);
	private:
		Camera::CameraType cameraType;
		float m_AspectRatio;//ÆÁÄ»¿í¸ß±È
		float m_ZoomLevel = 1.0f;
		Ref<Camera> m_Camera;
		bool m_Rotation;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;
		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraMoveSpeed = 5.0f;
		///////Perspective////////
		float m_pitch = 0.01f;
		float m_yaw = -90.0f;
		float m_sensitivity = 0.1f;
		glm::vec3 m_front = { 0.0f,0.0f,-1.0f };
		glm::vec3 m_up = {0.0f,1.0f,0.0f};
		float m_xpos = 0.0f;
		float m_ypos = 0.0f;
		bool m_firstMove = true;
	};
}
