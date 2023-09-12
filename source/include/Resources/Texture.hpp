#pragma once

#include <ResourcesManager.hpp>

class Texture : public IResource
{
private:
	int m_Width = 0, m_Height = 0, m_Channels = 0;
public:
	Texture() {};
	Texture(const char* _filepath) {
		LoadResource(_filepath);
	};
	~Texture() {
		UnloadResource();
	};

	unsigned int GetID() const;

	// Inherited via IResource
	virtual void LoadResource(const char* _name) override;
	virtual void UnloadResource() override;
};