#include <Texture.hpp>

Texture::Texture(const char* _filepath) {
	ResourceLoad(_filepath);
};

Texture::~Texture() {
	ResourceUnload();
};

unsigned int Texture::GetID() const {
	return m_ResourceId;
}

void Texture::ResourceFileRead(const std::string _name)
{
	std::filesystem::path path = "assets/textures/";
	path += _name;

	// Could be problematic on models
	stbi_set_flip_vertically_on_load(true);

	data = stbi_load(path.string().c_str(), &m_Width, &m_Height, &m_Channels, 0);
}

void Texture::ResourceLoadOpenGL(const std::string _name)
{
	if (data)
	{
		glGenTextures(1, &m_ResourceId);
		glActiveTexture(GL_TEXTURE0 + m_ResourceId);
		glBindTexture(GL_TEXTURE_2D, m_ResourceId);

		// Set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // MIPMAP is only for minifier
		GLenum format = 0;
		if (m_Channels == 1)
			format = GL_RED;
		else if (m_Channels == 3)
			format = GL_RGB;
		else if (m_Channels == 4)
			format = GL_RGBA;
		else
			Assert(true, std::string("Invalid number of channels in texture file ") + _name);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		DEBUG_WARNING("Failed to load Texture %s", _name.c_str());
	}
	stbi_image_free(data);
}

void Texture::ResourceUnload() {
	glDeleteTextures(1, &m_ResourceId);
}