#pragma once
class IResource
{
public:
	virtual void LoadResources(const char* _pathname) = 0;
};

class ResourcesManager
{
	//void LoadResource(IResource _toLoad);
};