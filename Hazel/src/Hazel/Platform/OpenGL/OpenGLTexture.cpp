#include "hzpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"


namespace Hazel
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		int width, height, channels;//宽度高度，和读取的图片有多少个通道
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		HZ_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if(channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if(channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;
		HZ_CORE_ASSERT(internalFormat && dataFormat, "Format not supported!");

		/*glGenTextures(1, &m_RenderID);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);*/
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RenderID);
		glTextureStorage2D(m_RenderID, 1, internalFormat, m_Width, m_Height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLACE);
		glTextureParameteri(m_RenderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RenderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RenderID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		:m_Width(width),m_Height(height)
	{
		m_internalFormat = GL_RGBA8, m_dataFormat = GL_RGBA;

		

		HZ_CORE_ASSERT(m_internalFormat && m_dataFormat, "Format not supported!");

		/*glGenTextures(1, &m_RenderID);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);*/
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RenderID);
		glTextureStorage2D(m_RenderID, 1, m_internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_S, GL_REPLACE);
		glTextureParameteri(m_RenderID, GL_TEXTURE_WRAP_T, GL_REPLACE);
		glTextureParameteri(m_RenderID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RenderID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RenderID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
		HZ_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture");
		glTextureSubImage2D(m_RenderID, 0, 0, 0, m_Width, m_Height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		//glBindTexture(GL_TEXTURE_2D, m_RenderID);
		glBindTextureUnit(0, m_RenderID);
	}
}
