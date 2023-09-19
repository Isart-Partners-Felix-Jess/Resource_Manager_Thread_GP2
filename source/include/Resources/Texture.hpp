#pragma once

#include <ResourcesManager.hpp>

class Texture : public IResource
{
private:
	int m_Width = 0, m_Height = 0, m_Channels = 0;
	unsigned char* data = nullptr;

public:
	Texture() {};
	Texture(const char* _filepath);
	~Texture();

	unsigned int GetID() const;

	// Inherited via IResource
	virtual void LoadResource(const std::string _name, bool isMultiThread = false) override;
	virtual void UnloadResource() override;

	void LoadResourceThreadJoined(const std::string _name) override;
};