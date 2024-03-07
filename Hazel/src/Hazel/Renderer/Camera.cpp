#include "hzpch.h"
#include "Camera.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
namespace Hazel
{
	Camera::CameraType Camera::s_CameraType = Camera::CameraType::Perspective;
}
