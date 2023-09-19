#pragma once

#include <string>

#include <Log.hpp>

class IResource
{
public:
	virtual ~IResource() = default;

	virtual void LoadResource(const std::string _name, bool isMultiThread = false) {
		ReadResourceFile(_name);

		if (!isMultiThread)
			LoadResourceThreaded(_name);

		isLoaded = true;
	}

	// To be defined by a class
	virtual void LoadResourceThreaded(const std::string _name) = 0;
	virtual void ReadResourceFile(const std::string _name) = 0;
	virtual void UnloadResource() = 0;

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

	bool isLoaded = false;

protected:
	unsigned int m_ResourceId = -1;
	std::string m_ResourcePath = "";
};