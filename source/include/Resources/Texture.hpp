#pragma once

#include <IResource.hpp>

#include <stb/stb_image.h>
#include <glad/glad.h>

#include <Assertion.hpp>

class Texture : public IResource
{
private:
	int m_width = -1, m_height = -1, m_channels = -1;
	unsigned char* m_data = nullptr;

public:
	Texture() {};
	~Texture();

	// Inherited via IResource
	void ResourceFileRead(const std::string _name);
	void ResourceLoadOpenGL(const std::string _name) override;
	void ResourceUnload() override;
};