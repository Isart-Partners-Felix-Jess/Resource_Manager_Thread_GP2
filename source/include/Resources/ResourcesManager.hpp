#pragma once
#include <string>
#include <unordered_map>

class IResource
{
public:
	virtual ~IResource() = default;
	virtual void LoadResource(const char* _pathname) = 0;
	virtual void UnloadResource() = 0;

	void SetResourceId(size_t _id);
	size_t GetResourceId() const;

	void SetResourcePath(const std::string& _path);
	std::string GetResourcePath() const;

protected:
	size_t m_ResourceId;
	std::string m_ResourcePath;
};

//Trying static instead of singleton, both are relevant here
static class ResourcesManager
{

	void LoadResource(IResource* _toLoad);

public:
	void Destroy();

	template<typename R>
	//Maybe try to make the parameter a path...
	R* CreateResource(std::string _name);
	template<typename R>
	R* GetResource(std::string _name);
	void Delete(std::string _name);
private:
	std::unordered_map<std::string, IResource*> m_Resources;
};

template<typename R>
inline R* ResourcesManager::CreateResource(std::string _name)
{
	R* createdResource = new R();
	createdResource->LoadResource();
	m_Resources.insert(std::make_pair( _name, createdResource ));
	return createdResource;
}

template<typename R>
inline R* ResourcesManager::GetResource(std::string _name)
{
	return nullptr;
}
