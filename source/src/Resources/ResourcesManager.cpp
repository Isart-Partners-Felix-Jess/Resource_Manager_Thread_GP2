#include <ResourcesManager.hpp>
#include <Model.hpp>


//Unused for now
void ResourcesManager::LoadResource(IResource* _toLoad)
{
	_toLoad->LoadResource("");
}

ResourcesManager::ResourcesManager()
{
}

ResourcesManager::~ResourcesManager()
{
	Destroy();
}

void ResourcesManager::Destroy()
{
	for (std::pair<std::string, IResource*> pair : m_Resources)
	{
		DEBUG_LOG("Resource %s deleted successfully", pair.first);
		delete pair.second;
	}
	m_Resources.clear(); //Probably useless
	DEBUG_LOG("Resource manager cleared successfully");
}

ResourcesManager& ResourcesManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ResourcesManager();
	}
	return *instance;
}

void ResourcesManager::Delete(std::string _name)
{
	auto it = m_Resources.find(_name);
	if (it == m_Resources.end())
	{
		DEBUG_ERROR("Resource %s not found; Could not delete %s", _name, _name);
		return;
	}
	delete m_Resources.find(_name)->second;
	m_Resources.erase(_name);
	DEBUG_LOG("Resource %s deleted successfully", _name);
}

void IResource::SetResourceId(size_t _id)
{
	m_ResourceId = _id;
}

size_t IResource::GetResourceId() const
{
	if (m_ResourceId)
	{
		DEBUG_WARNING("Could not find id for %s ", m_ResourcePath.c_str());
	}
	return m_ResourceId;
}

void IResource::SetResourcePath(const std::string& _path)
{
	m_ResourcePath = _path;
}

std::string IResource::GetResourcePath() const
{
	if (m_ResourcePath.empty())
	{
		DEBUG_WARNING("Could not find path for %i ", m_ResourceId);
	}
	return m_ResourcePath;
}
