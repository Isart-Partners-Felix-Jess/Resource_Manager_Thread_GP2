#pragma once
#include <string>
#include <unordered_map>
#include <Log.hpp>

class IResource
{
public:
	virtual ~IResource() = default;
	virtual void LoadResource(const char* _name) = 0;
	virtual void UnloadResource() = 0;

	void SetResourceId(size_t _id);
	size_t GetResourceId() const;

	void SetResourcePath(const std::string& _path);
	std::string GetResourcePath() const;

protected:
	size_t m_ResourceId;
	std::string m_ResourcePath;
};

//Trying static instead of singleton, both are relevant here
static class ResourcesManager
{
	//Unused for now
	void LoadResource(IResource* _toLoad);

public:
	void Destroy();

	template<typename R>
	//Maybe try to make the parameter a path...
	R* CreateResource(std::string _name);
	template<typename R>
	R* GetResource(std::string _name);
	void Delete(std::string _name);
private:
	std::unordered_map<std::string, IResource*> m_Resources;
};

template<typename R>
inline R* ResourcesManager::CreateResource(std::string _name)
{
	IResource* createdResource = new R();
	createdResource->SetResourcePath(_name);
	createdResource->SetResourceId(m_Resources.size());
	createdResource->LoadResource(_name);
	m_Resources.emplace(_name, createdResource);
	DEBUG_LOG("Resource %s created, ID: %i", _name, createdResource->GetResourceId());
	return createdResource;
}

template<typename R>
inline R* ResourcesManager::GetResource(std::string _name)
{
	auto it = m_Resources.find(_name);
	if (it != m_Resources.end())
	{
		// Found the resource, return the raw pointer
		DEBUG_LOG("Resource %s loaded", _name);
		return it->second; //No dynamic_cast, daring
	}
	else
	{
		DEBUG_LOG("Resource %s not found", _name)
		// Resource not found, return nullptr
		return nullptr;
	}
}
