#include "stdafx.h"
#include "ResourceManager.h"
bool ResourceManager::RemoveResource(ResourceType type, std::string globalName)
{
	if (type >= RT_MAX) return false;
	ResourceList::iterator iter = m_vecResources[type].find(globalName);
	if (iter != m_vecResources[type].end())
	{
		m_vecResources[type].erase(iter);
		return true;
	}
	return false;
}
Resource* ResourceManager::LoadResource(ResourceType type, std::string globalName)
{
	if (type >= RT_MAX) return NULL;
	ResourceList::iterator iter = m_vecResources[type].find(globalName);
	if (iter != m_vecResources[type].end())
	{
		iter->second->AddRef();
		return iter->second;
	}
	//没找到需要硬盘加载
	//todo
	return NULL;
}