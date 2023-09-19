#pragma once

#include <string>

#include <Log.hpp>

class IResource
{
public:
	bool isLoaded = false;
	virtual ~IResource() = default;

	virtual void ResourceLoad(const std::string _name, bool isMultiThread = false) {
		ResourceFileRead(_name);

		if (!isMultiThread)
			ResourceLoadOpenGL(_name);

		isLoaded = true;
	}

	// To be defined by a class
	virtual void ResourceFileRead(const std::string _name) = 0;
	virtual void ResourceLoadOpenGL(const std::string _name) = 0;
	virtual void ResourceUnload() = 0;

	inline unsigned int GetResourceId() const {
		if (m_ResourceId == static_cast<unsigned int>(-1))
			DEBUG_WARNING("Could not find id for %s ", m_ResourcePath.c_str());
		return m_ResourceId;
	}
	
	inline void SetResourcePath(const std::string& _path) {
		m_ResourcePath = _path;
	}

	inline std::string GetResourcePath() const
	{
		if (m_ResourcePath.empty())
			DEBUG_WARNING("Could not find path for %i ", m_ResourceId);
		return m_ResourcePath;
	}

protected:
	unsigned int m_ResourceId = -1;
	std::string m_ResourcePath = "";
};