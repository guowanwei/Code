#pragma once
#include <string>
enum ResourceType
{
	RT_MATERIAL,  //
	RT_TEXTURE,
	RT_MESH,
	RT_MAX
};
class Resource
{
public:
	
private:
	const ResourceType m_enumType;
	const std::string m_strGlobalName;
	int m_intRefCount;   //引用计数
public:
	Resource(ResourceType type, std::string globalName);
	~Resource();
	void AddRef() {
		++m_intRefCount;
	}
	void Release();
	
};