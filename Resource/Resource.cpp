#include "stdafx.h"
#include "Resource.h"
#include "ResourceManager.h"
Resource::Resource(ResourceType type,std::string globalName)
	:m_enumType(type)
	, m_intRefCount(1)
	, m_strGlobalName(globalName)
{
}
Resource::~Resource()
{

}
void Resource::Release()
{
	--m_intRefCount;
	if (0 == m_intRefCount)
	{
		//1.remove from manager
		ResourceManager::Instance().RemoveResource(m_enumType, m_strGlobalName);
		// delete
		delete this;
	}
}