#pragma once
#include <string>
#include <glm/glm.hpp>


#include "Hazel/Renderer/Shader.h"
typedef unsigned int GLenum;
namespace Hazel
{
	class OpenGLShader:public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filepath);
		virtual  ~OpenGLShader();
		virtual void Bind() const;
		virtual void UnBind() const;
		virtual  const std::string& GetName() const override { return m_Name; }
		void UploadUniformInt(const int value, const std::string& name);

		void UploadUniformFloat(const float value, const std::string& name);
		void UploadUniformFloat2(const glm::vec2& value, const std::string& name);
		void UploadUniformFloat3(const glm::vec3& value, const std::string& name);
		void UploadUniformFloat4(const glm::vec4& value, const std::string& name);

		void UploadUniformMat3(const glm::mat3& matrix, const std::string& name);
		void UploadUniformMat4(const glm::mat4& matrix, const std::string& name);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum,std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}


