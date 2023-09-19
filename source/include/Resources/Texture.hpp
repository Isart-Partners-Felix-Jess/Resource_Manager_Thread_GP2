#pragma once

#include <IResource.hpp>

#include <stb/stb_image.h>
#include <glad/glad.h>

#include <Assertion.hpp>

class Texture : public IResource
{
private:
	int m_Width = -1, m_Height = -1, m_Channels = -1;
	unsigned char* data = nullptr;

public:
	Texture() {};
	Texture(const char* _filepath);
	~Texture();

	unsigned int GetID() const;

	void ResourceFileRead(const std::string _name);

	// Inherited via IResource
	void ResourceLoadOpenGL(const std::string _name) override;
	virtual void ResourceUnload() override;
};