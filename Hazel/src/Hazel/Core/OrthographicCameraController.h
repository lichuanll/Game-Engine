#pragma once
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Timestep.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
namespace Hazel
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera()const { return m_Camera; }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel() { return m_ZoomLevel; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		bool m_Rotation;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;
		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraMoveSpeed = 5.0f;
	};
}
