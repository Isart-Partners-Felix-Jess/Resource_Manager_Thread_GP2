#include <ResourcesManager.hpp>
#include <Model.hpp>
#include <thread>

// Singleton
std::atomic<ResourcesManager*> ResourcesManager::m_Instance = nullptr;
std::mutex ResourcesManager::m_Mutex;
std::unordered_map<std::string, IResource*> ResourcesManager::m_Resources;
ThreadPool ResourcesManager::m_ThreadPool;

ResourcesManager::ResourcesManager() {
	m_Instance = this->GetInstance();
}

ResourcesManager::~ResourcesManager()
{
	Destroy();
	delete m_Instance;
}

bool ResourcesManager::isPoolDone()
{
	unsigned int totalDone = 0;
	for (std::pair<std::string, IResource*> pair : m_Resources)
		if (pair.second->isLoaded) totalDone++;

	if (totalDone == m_Resources.size())
		return true;
	else
		return false;
}

void ResourcesManager::Destroy()
{
	Log::SuccessColor();
	for (std::pair<std::string, IResource*> pair : m_Resources)
	{
		pair.second->UnloadResource();
		delete pair.second;
		DEBUG_LOG("Resource %s deleted successfully", pair.first.c_str());
	}
	m_Resources.clear(); // Probably useless
	Model::ResetCount();
	Shader::ResetCount();
	DEBUG_LOG("Resource manager cleared successfully");
	Log::ResetColor();
}

ResourcesManager* ResourcesManager::GetInstance()
{
	ResourcesManager* instance = m_Instance.load(std::memory_order::acquire);
	if (instance == nullptr)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		instance = m_Instance.load(std::memory_order::relaxed);
		if (instance == nullptr)
			instance = new ResourcesManager();
	}
	return instance;
}

void ResourcesManager::Delete(const std::string& _name)
{
	auto it = m_Resources.find(_name);
	if (it == m_Resources.end())
	{
		DEBUG_ERROR("Resource %s not found, could not be deleteted", _name);
		return;
	}

	delete m_Resources.find(_name)->second;
	m_Resources.erase(_name);

	Log::SuccessColor();
	DEBUG_LOG("Resource %s deleted successfully", _name);
	Log::ResetColor();
}

unsigned int IResource::GetResourceId() const
{
	if (m_ResourceId == static_cast<unsigned int>(-1))
		DEBUG_WARNING("Could not find id for %s ", m_ResourcePath.c_str());
	return m_ResourceId;
}

void IResource::SetResourcePath(const std::string& _path) {
	m_ResourcePath = _path;
}

std::string IResource::GetResourcePath() const
{
	if (m_ResourcePath.empty())
		DEBUG_WARNING("Could not find path for %i ", m_ResourceId);
	return m_ResourcePath;
}