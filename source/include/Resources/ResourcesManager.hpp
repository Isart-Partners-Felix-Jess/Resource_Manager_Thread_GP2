#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <future>

#include <Log.hpp>
#include <../Thread/ThreadPool.hpp>

class IResource
{
public:
	virtual ~IResource() = default;

	virtual void LoadResource(const std::string _name, bool isMultiThread = false) = 0;
	virtual void UnloadResource() = 0;

	virtual void LoadResourceThreaded(const std::string _name) = 0;

	unsigned int GetResourceId() const;

	void SetResourcePath(const std::string& _path);
	std::string GetResourcePath() const;

	bool isLoaded = false;

protected:
	unsigned int m_ResourceId = 0;
	std::string m_ResourcePath;
};

class ResourcesManager
{
private:
	static std::atomic<ResourcesManager*> m_Instance;
	static std::mutex m_Mutex;
	static std::unordered_map<std::string, IResource*> m_Resources;

	static ThreadPool m_ThreadPool;

	ResourcesManager();
	~ResourcesManager();

public:
	static ResourcesManager* GetInstance();

	// Maybe try to make the parameter a path...
	template<typename R>
	static R* CreateResource(const std::string& _name, bool isMultiThread = false)
	{
		IResource* createdResource = new R();
		if (!isMultiThread) {
			createdResource->SetResourcePath(_name);
			createdResource->LoadResource(_name);

			// Erase previous pointer if found
			auto it = m_Resources.find(_name);
			if (it != m_Resources.end())
				delete it->second;

			m_Resources.emplace(_name, createdResource);
		}
		else
		{
			auto it = m_Resources.find(_name);
			if (it == m_Resources.end())
			{
				DEBUG_WARNING("Resource %s not found", _name.c_str());
				return nullptr;
			}
			createdResource = it->second;
			createdResource->LoadResourceThreaded(_name);
		}
		DEBUG_LOG("Resource %s loaded, ID: %i", _name.c_str(), createdResource->GetResourceId());
		return dynamic_cast<R*>(createdResource);
	}

	template<typename R>
	static void CreateResourceThreaded(const std::string& _name)
	{
		IResource* createdResource = new R();
		createdResource->SetResourcePath(_name);

		m_ThreadPool.addToQueue([createdResource, _name]() { createdResource->LoadResource(_name, true); });

		auto it = m_Resources.find(_name);
		if (it != m_Resources.end())
			delete it->second;
		m_Resources.emplace(_name, createdResource);
	}

	template<typename R>
	static R* GetResource(const std::string& _name)
	{
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

	static bool isPoolDone();

	static void Destroy();
	void Delete(const std::string& _name);
};