#include "hzpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Hazel
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		HZ_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;
		glGenTextures(1, &m_RenderID);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_RenderID);
		//glTextureStorage2D(m_RenderID, 1, GL_RGB8, m_Width, m_Height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLACE);
		glTextureParameteri(m_RenderID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RenderID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glTextureSubImage2D(m_RenderID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RenderID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		//glBindTexture(GL_TEXTURE_2D, m_RenderID);
		//glBindTextureUnit(0, m_RenderID);
	}
}
