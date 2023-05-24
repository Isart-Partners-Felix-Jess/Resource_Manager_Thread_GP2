#include <Texture.hpp>
#include <stb/stb_image.h>
#include <glad/glad.h>

Texture::Texture()
{}

Texture::Texture(const char* _filepath, bool _alphaChannel)
{
	std::filesystem::path path = "assets/textures/";
	path += _filepath;
	//Could be problematic on models
	stbi_set_flip_vertically_on_load(true);
	//Potential class members
	unsigned char* data = stbi_load(path.string().c_str(), &m_Width, &m_Height, &m_Channels, 0);
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_2D, m_Id);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // MIPMAP is only for minifier
	if (data)
	{
		if (_alphaChannel)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
									//May be RGBA if needed
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		DEBUG_WARNING("Failed to load Texture %s", _filepath);
	}
	stbi_image_free(data);

}
Texture::~Texture()
{
}

unsigned int Texture::GetID() const
{
	return m_Id;
}

void Texture::LoadResource(const char* _name)
{
}

void Texture::UnloadResource()
{
}
