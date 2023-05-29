#pragma once

#ifndef RESSOURCESMANAGER_H
#define RESSOURCESMANAGER_H

#include <string>
#include <unordered_map>
#include <Log.hpp>

class IResource
{
public:
	virtual ~IResource() = default;
	virtual void LoadResource(const char* _name) = 0;
	virtual void UnloadResource() = 0;

	//void SetResourceId(size_t _id);
	unsigned int GetResourceId() const;

	void SetResourcePath(const std::string& _path);
	std::string GetResourcePath() const;

public:
	unsigned int m_ResourceId = -1;
	std::string m_ResourcePath;
};

class ResourcesManager
{
private:
	//Unused for now
	static ResourcesManager* instance;
	static std::unordered_map<std::string, IResource*> m_Resources;
	ResourcesManager();
	~ResourcesManager();
public:
	static ResourcesManager* GetInstance();
	template<typename R>
	//Maybe try to make the parameter a path...
	static R* CreateResource(const std::string& _name);
	template<typename R>
	static R* GetResource(const std::string& _name);
	static void Destroy();
	void Delete(const std::string& _name);
private:
	void LoadResource(IResource* _toLoad);
};

template<typename R>
inline R* ResourcesManager::CreateResource(const std::string& _name)
{
	IResource* createdResource = new R();
	createdResource->SetResourcePath(_name);
	//createdResource->SetResourceId(m_Resources.size());
	createdResource->LoadResource(_name.c_str());
	//Erase previous pointer if found
	auto it = m_Resources.find(_name);
	if (it != m_Resources.end())
		delete it->second;

	m_Resources.emplace(_name, createdResource);
	DEBUG_LOG("Resource %s created, ID: %i", _name.c_str(), createdResource->GetResourceId());
	return dynamic_cast<R*>(createdResource);
}

template<typename R>
inline R* ResourcesManager::GetResource(const std::string& _name)
{
	auto it = m_Resources.find(_name);
	if (it != m_Resources.end())
	{
		// Found the resource, return the raw pointer
		DEBUG_LOG("Resource %s loaded", _name.c_str());
		return dynamic_cast<R*>(it->second); //No dynamic_cast, daring
	}
	else
	{
		DEBUG_WARNING("Resource %s not found", _name);
		// Resource not found, return nullptr
		return nullptr;
	}
}
#endif //RESSOURCESMANAGER_H