#include "SandBox2D.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <chrono>
template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Func({ m_Name, duration });
	}
private:
	const char* m_Name;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");
	//Timer<>timer("Sandbox2D::OnUpdate");
	{
		PROFILE_SCOPE("Camera::OnUpdate");
		m_Camera = m_CameraController.GetCamera();
		m_CameraController.ChangeCameraType(Hazel::Camera::CameraType::Orthographic);
		m_CameraController.OnUpdate(ts);
	}
	{
		PROFILE_SCOPE("Renderer Prepare");
		Hazel::RenderCommand::SetClearColor({ 0,0,0,1 });
		Hazel::RenderCommand::Clear();
	}
	
	//Hazel::RenderCommand::SetMouseMode(Hazel::Application::Get().GetWindow().GetNativeWindow(), true);
	//m_CameraController.ChangeCameraType(Hazel::Camera::CameraType::Orthographic);
	Hazel::Renderer2D::BeginScene(m_Camera);
	{
		PROFILE_SCOPE("Renderer Draw");
		

		Hazel::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 0.8f,0.8f }, { 0.8f,0.2f,0.3f,1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.5f,-0.5f }, { 0.5f,0.75f }, { 0.2f,0.3f,0.8f,1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 10.0f,10.0f }, m_Texture);
		//glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		//glm::vec4 BlueColor(0.2f, 0.3f, 0.8f, 1.0f);

		/*std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4(m_SquareColor, "u_Color");*/


		//Hazel::Renderer::Submit(m_SquareVA, m_FlatColorShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));




		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Setting");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	for(auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, result.Name);
		strcat(label, " %.3fms");
		ImGui::Text(label, result.Time);
	}

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
