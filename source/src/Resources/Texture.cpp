#include <Texture.hpp>

Texture::~Texture() {
	ResourceUnload();
};

void Texture::ResourceFileRead(const std::string _name)
{
	std::filesystem::path path = "assets/textures/";
	path += _name;

	// Could be problematic on models
	stbi_set_flip_vertically_on_load(true);

	m_data = stbi_load(path.string().c_str(), &m_width, &m_height, &m_channels, 0);

	m_isRead = true;
}

void Texture::ResourceLoadOpenGL(const std::string _name)
{
	if (m_data)
	{
		glGenTextures(1, &m_resourceId);
		glActiveTexture(GL_TEXTURE0 + m_resourceId);
		glBindTexture(GL_TEXTURE_2D, m_resourceId);

		// Set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // MIPMAP is only for minifier
		GLenum format = 0;
		if (m_channels == 1)
			format = GL_RED;
		else if (m_channels == 3)
			format = GL_RGB;
		else if (m_channels == 4)
			format = GL_RGBA;
		else
			Assert(true, std::string("Invalid number of channels in texture file ") + _name);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		DEBUG_WARNING("Failed to load Texture %s", _name.c_str());
	}
	stbi_image_free(m_data);
	m_isLoaded = true;
}

void Texture::ResourceUnload() {
	glDeleteTextures(1, &m_resourceId);
}