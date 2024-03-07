#pragma once
#include <glm/glm.hpp>

namespace Hazel
{
	class Camera
	{
	public:
		enum class CameraType
		{
			Perspective,Orthographic
		};
	public:
		virtual void SetProjection(glm::mat4 ProjectionMatrix) = 0;
		virtual const glm::vec3& GetPosition()const = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;
		virtual float GetRotation()const = 0;
		virtual void SetRotation(float rotation) = 0;
		virtual void SetPerspectiveRotation(glm::vec3 pos, glm::vec3 front, glm::vec3 up) = 0;
		virtual const glm::mat4& GetProjectionMatrix()const = 0;
		virtual const glm::mat4& GetViewMatrix()const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix()const = 0;
	public:
		inline static CameraType GetCameraType() { return s_CameraType; }
		inline static void SetCameraType(CameraType cameraType) { s_CameraType = cameraType; }
		static Ref<Camera> Create();
		
	private:
		static CameraType s_CameraType;
	};
}
