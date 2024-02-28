#include<Hazel.h>

#include "imgui/imgui.h"

class ExampleLayer :public Hazel::Layer 
{
public:
	ExampleLayer() :Layer("Example"),
		 m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());



		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f
		};
		std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));//为VBO分配缓存
		//放入代码块，让这个layout执行完就自动销毁

		Hazel::BufferLayout layout = {
		{Hazel::ShaderDataType::Float3, "a_Position"},
		{Hazel::ShaderDataType::Float4, "a_Color"}

		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		unsigned int indices[3] = { 0,1,2 };
		std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_SquareVA.reset(Hazel::VertexArray::Create());
		float SquareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));

		squareVB->SetLayout({
			{Hazel::ShaderDataType::Float3, "a_Position" }
			}
		);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int SquareIndices[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(SquareIndices, sizeof(SquareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color=a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
		m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));
		std::string vertexSrc2 = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				
			}
		)";
		m_Shader2.reset(new Hazel::Shader(vertexSrc2, fragmentSrc2));
	}
	void OnUpdate()override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed;
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
		{
			m_CameraPosition.y += m_CameraMoveSpeed;
		}

		if(Hazel::Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraRotation -= m_CameraRotationSpeed;
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraRotation += m_CameraRotationSpeed;
		}
		Hazel::RenderCommand::SetClearColor({ 1,0,1,1 });
		Hazel::RenderCommand::Clear();
		Hazel::Renderer::BeginScene(m_Camera);

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		Hazel::Renderer::Submit(m_SquareVA, m_Shader2);


		Hazel::Renderer::Submit(m_VertexArray, m_Shader);


		Hazel::Renderer::EndScene();
	}
	void OnEvent(Hazel::Event& event)override
	{
		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
		
	}
	virtual  void OnImGuiRender() override
	{
		
	}
	bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event)
	{
		
		return false;
	}
private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::Shader> m_Shader2;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.02f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 0.5f;
};
class Sandbox :public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}

};
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox;
}