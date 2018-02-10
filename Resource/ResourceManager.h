#pragma once
#include <map>
#include <vector>
#include "Resource.h"
class Resource;
class ResourceManager
{
	//data
private:
	typedef std::map<std::string, Resource*> ResourceList;
	std::vector<ResourceList> m_vecResources;


private:
	ResourceManager()
		:m_vecResources(RT_MAX)
	{
	}
public:
	static ResourceManager& Instance()
	{
		static ResourceManager manager;
		return manager;
	}
	
	

	Resource* LoadResource(ResourceType type, std::string globalName);
	bool RemoveResource(ResourceType type,std::string globalName);
};