#pragma once

#include <string>

#include <Log.hpp>
#include <atomic>

class IResource
{
public:
	bool isRead = false;
	bool isLoaded = false;
	virtual ~IResource() = default;

	// To be defined by a class
	virtual void ResourceFileRead(const std::string _name) = 0;
	virtual void ResourceLoadOpenGL(const std::string _name) = 0;
	virtual void ResourceUnload() = 0;

	inline unsigned int GetResourceId() const {
		if (m_resourceId == static_cast<unsigned int>(-1))
			DEBUG_WARNING("Could not find id for %s ", m_resourcePath.c_str());
		return m_resourceId;
	}

	inline void SetResourcePath(const std::string& _path) {
		m_resourcePath = _path;
	}

	inline std::string GetResourcePath() const
	{
		if (m_resourcePath.empty())
			DEBUG_WARNING("Could not find path for %i ", m_resourceId);
		return m_resourcePath;
	}

protected:
	unsigned int m_resourceId = -1;
	std::string m_resourcePath = "";
};