#include "SandBox2D.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{
	
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	m_Camera = m_CameraController.GetCamera();
	m_CameraController.ChangeCameraType(Hazel::Camera::CameraType::Orthographic);
	m_CameraController.OnUpdate(ts);
	Hazel::RenderCommand::SetClearColor({ 0,0,0,1 });
	Hazel::RenderCommand::Clear();
	//Hazel::RenderCommand::SetMouseMode(Hazel::Application::Get().GetWindow().GetNativeWindow(), true);
	//m_CameraController.ChangeCameraType(Hazel::Camera::CameraType::Orthographic);
	Hazel::Renderer2D::BeginScene(m_Camera);

	Hazel::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 0.8f,0.8f }, { 0.8f,0.2f,0.3f,1.0f });
	Hazel::Renderer2D::DrawQuad({ 0.5f,-0.5f }, { 0.5f,0.75f }, { 0.2f,0.3f,0.8f,1.0f });
	//glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
	//glm::vec4 BlueColor(0.2f, 0.3f, 0.8f, 1.0f);

	/*std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4(m_SquareColor, "u_Color");*/
	

	//Hazel::Renderer::Submit(m_SquareVA, m_FlatColorShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	
	


	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Setting");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
