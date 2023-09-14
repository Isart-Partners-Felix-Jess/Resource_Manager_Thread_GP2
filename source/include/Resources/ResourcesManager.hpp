#pragma once

#include <string>
#include <unordered_map>
#include <mutex>

#include <Log.hpp>

class IResource
{
public:
	virtual ~IResource() = default;

	virtual void LoadResource(const char* _name) = 0;
	virtual void UnloadResource() = 0;

	void LoadResourceThread(const char* _name);

	unsigned int GetResourceId() const;

	void SetResourcePath(const std::string& _path);
	std::string GetResourcePath() const;

protected:
	unsigned int m_ResourceId = -1;
	std::string m_ResourcePath;
};

class ResourcesManager
{
private:
	static std::atomic<ResourcesManager*> m_instance;
	static std::mutex m_mutex;
	static std::unordered_map<std::string, IResource*> m_Resources;

	ResourcesManager();
	~ResourcesManager();

public:
	static ResourcesManager* GetInstance();

	// Maybe try to make the parameter a path...
	template<typename R>
	static R* CreateResource(const std::string& _name) {
		IResource* createdResource = new R();
		createdResource->SetResourcePath(_name);
		createdResource->LoadResourceThread(_name.c_str());
		// Erase previous pointer if found
		auto it = m_Resources.find(_name);
		if (it != m_Resources.end())
			delete it->second;

		m_Resources.emplace(_name, createdResource);
		DEBUG_LOG("Resource %s created, ID: %i", _name.c_str(), createdResource->GetResourceId());
		return dynamic_cast<R*>(createdResource);
	}
	template<typename R>
	static R* CreateResourceThread(const std::string& _name) {
		R* createdResource = new R();
		std::thread creationThread = std::thread([_name,createdResource]()mutable { createdResource = CreateResource<R>(_name); });
		return createdResource;
	}
	template<typename R>
	static R* GetResource(const std::string& _name) {
		auto it = m_Resources.find(_name);
		if (it != m_Resources.end())
		{
			// Found the resource, return the raw pointer
			DEBUG_LOG("Resource %s loaded", _name.c_str());
			return dynamic_cast<R*>(it->second);
		}
		else
		{
			// Resource not found, return nullptr
			DEBUG_WARNING("Resource %s not found", _name);
			return nullptr;
		}
	}

	static void Destroy();
	void Delete(const std::string& _name);

private:
	void LoadResource(IResource* _toLoad);
};