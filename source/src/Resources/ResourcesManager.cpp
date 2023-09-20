#include <ResourcesManager.hpp>

// Singleton
std::atomic<ResourcesManager*> ResourcesManager::s_m_instance = nullptr;
std::mutex ResourcesManager::s_m_mutex;
std::unordered_map<std::string, IResource*> ResourcesManager::s_m_resources;
ThreadPool ResourcesManager::s_m_threadPool;
bool ResourcesManager::s_m_isDeadPool = false;

ResourcesManager::ResourcesManager() {
	s_m_instance = this->GetInstance();
}

ResourcesManager::~ResourcesManager()
{
	Destroy();
	delete s_m_instance;
}

bool ResourcesManager::IsPoolDone()
{
	//if (s_m_isDeadPool)
		//return true;

	unsigned int totalDone = 0;
	for (std::pair<std::string, IResource*> pair : s_m_resources)
		if (pair.second->isLoaded) totalDone++;

	if (totalDone == s_m_resources.size())
	{
		s_m_isDeadPool = true;
		return true;
	}
	else
		return false;
}

void ResourcesManager::Destroy()
{
	Log::SuccessColor();
	for (std::pair<std::string, IResource*> pair : s_m_resources)
	{
		pair.second->ResourceUnload();
		delete pair.second;
		DEBUG_LOG("Resource %s deleted successfully", pair.first.c_str());
	}
	s_m_resources.clear(); // Probably useless
	Model::ResetCount();
	Shader::ResetCount();
	DEBUG_LOG("Resource manager cleared successfully");
	Log::ResetColor();
}

ResourcesManager* ResourcesManager::GetInstance()
{
	ResourcesManager* instance = s_m_instance.load(std::memory_order::acquire);
	if (instance == nullptr)
	{
		std::lock_guard<std::mutex> lock(s_m_mutex);
		instance = s_m_instance.load(std::memory_order::relaxed);
		if (instance == nullptr)
			instance = new ResourcesManager();
	}
	return instance;
}

void ResourcesManager::Delete(const std::string& _name)
{
	auto it = s_m_resources.find(_name);
	if (it == s_m_resources.end())
	{
		DEBUG_ERROR("Resource %s not found, could not be deleteted", _name);
		return;
	}

	delete s_m_resources.find(_name)->second;
	s_m_resources.erase(_name);

	Log::SuccessColor();
	DEBUG_LOG("Resource %s deleted successfully", _name);
	Log::ResetColor();
}