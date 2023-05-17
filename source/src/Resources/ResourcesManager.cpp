#include <ResourcesManager.hpp>
#include <Model.hpp>


void ResourcesManager::LoadResource(IResource* _toLoad)
{
	_toLoad->LoadResource("");
}

void ResourcesManager::Destroy()
{
	for (std::pair<std::string, IResource*> pair : m_Resources)
	{
		delete pair.second;
	}
}

void ResourcesManager::Delete(std::string _name)
{
	delete m_Resources.find(_name)->second;
	m_Resources.erase(_name);
}

void IResource::SetResourceId(size_t _id)
{
	m_ResourceId = _id;
}

size_t IResource::GetResourceId() const
{
	return m_ResourceId;
}

void IResource::SetResourcePath(const std::string& _path)
{
	m_ResourcePath = _path;
}

std::string IResource::GetResourcePath() const
{
	return m_ResourcePath;
}
