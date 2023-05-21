#include <ResourcesManager.hpp>
//#include <Model.hpp>

//Singleton
ResourcesManager* ResourcesManager::instance = nullptr;
std::unordered_map<std::string, IResource*> ResourcesManager::m_Resources; //Erik: Necessary ?


//Unused for now
void ResourcesManager::LoadResource(IResource* _toLoad)
{
	_toLoad->LoadResource("");
}

ResourcesManager::ResourcesManager()
{
	instance = this->GetInstance();
}

ResourcesManager::~ResourcesManager()
{
	Destroy();
	delete instance;
}

void ResourcesManager::Destroy()
{
	Log::SuccessColor();
	for (std::pair<std::string, IResource*> pair : m_Resources)
	{
		DEBUG_LOG("Resource %s deleted successfully", pair.first.c_str());
		pair.second->UnloadResource();
		delete pair.second;
	}
	m_Resources.clear(); //Probably useless
	DEBUG_LOG("Resource manager cleared successfully");
	Log::ResetColor();
}

ResourcesManager* ResourcesManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ResourcesManager();
	}
	return instance;
}

void ResourcesManager::Delete(std::string _name)
{
	const char* name = _name.c_str();
	auto it = m_Resources.find(_name);
	if (it == m_Resources.end())
	{
		DEBUG_ERROR("Resource %s not found; Could not delete %s", name, name);
		return;
	}
	delete m_Resources.find(_name)->second;
	m_Resources.erase(_name);
	Log::SuccessColor();
	DEBUG_LOG("Resource %s deleted successfully", name);
	Log::ResetColor();
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
