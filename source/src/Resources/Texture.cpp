#include <Texture.hpp>
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <Assertion.hpp>

Texture::Texture()
{}

Texture::Texture(const char* _filepath)
{
	LoadResource(_filepath);
}
Texture::~Texture()
{
}

unsigned int Texture::GetID() const
{
	return m_ResourceId;
}

void Texture::LoadResource(const char* _name)
{
	std::filesystem::path path = "assets/textures/";
	path += _name;
	//Could be problematic on models
	stbi_set_flip_vertically_on_load(true);
	//Potential class members
	unsigned char* data = stbi_load(path.string().c_str(), &m_Width, &m_Height, &m_Channels, 0);
	if (data)
	{
	glGenTextures(1, &m_ResourceId);
	glActiveTexture(GL_TEXTURE0 + m_ResourceId-1);
	glBindTexture(GL_TEXTURE_2D, m_ResourceId);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // MIPMAP is only for minifier
		GLenum format;
		if (m_Channels == 1)
			format = GL_RED;
		else if (m_Channels == 3)
			format = GL_RGB;
		else if (m_Channels == 4)
			format = GL_RGBA;
		else
			Assert(true, "Invalid number of channels in texture file " << _name);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		DEBUG_WARNING("Failed to load Texture %s", _name);
	}
	stbi_image_free(data);
}

void Texture::UnloadResource()
{
}
