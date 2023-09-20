#pragma once

#include <unordered_map>

#include <Log.hpp>
#include <Model.hpp>

#include <ThreadPool.hpp>
#include <IResource.hpp>

class ResourcesManager
{
private:
	static std::atomic<ResourcesManager*> s_m_instance;
	static std::mutex s_m_mutex;
	static std::unordered_map<std::string, IResource*> s_m_resources;
	static ThreadPool s_m_threadPool;

	ResourcesManager();
	~ResourcesManager();

public:
	static ResourcesManager* GetInstance();

	// Maybe try to make the parameter a path...
	template<typename R>
	static R* CreateResource(const std::string& _name, bool _isMultiThread)
	{
		IResource* createdResource = new R();
		if (!_isMultiThread)
		{
			createdResource->SetResourcePath(_name);
			createdResource->ResourceFileRead(_name);

			// Erase previous pointer if found
			auto it = s_m_resources.find(_name);
			if (it != s_m_resources.end())
				delete it->second;

			s_m_resources.emplace(_name, createdResource);
		}
		else
		{
			auto it = s_m_resources.find(_name);
			if (it == s_m_resources.end())
			//{
				//DEBUG_WARNING("Resource %s not found", _name.c_str());
				return nullptr;
			//}
			createdResource = it->second;
			//createdResource->ResourceLoadOpenGL(_name);
		}
		DEBUG_LOG("Resource %s loaded, ID: %i", _name.c_str(), createdResource->GetResourceId());
		return dynamic_cast<R*>(createdResource);
	}

	template<typename R>
	static void CreateResourceThreaded(const std::string& _name)
	{
		IResource* createdResource = new R();
		createdResource->SetResourcePath(_name);

		s_m_threadPool.AddToQueue([createdResource, _name]() { createdResource->ResourceFileRead(_name); }, _name + " creation");

		auto it = s_m_resources.find(_name);
		if (it != s_m_resources.end())
			delete it->second;
		s_m_resources.emplace(_name, createdResource);
	}

	template<typename R>
	static R* GetResource(const std::string& _name)
	{
		auto it = s_m_resources.find(_name);
		if (it != s_m_resources.end())
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

	static bool IsPoolDone();

	static void Destroy();
	void Delete(const std::string& _name);
};