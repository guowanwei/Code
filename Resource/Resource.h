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
	int m_intRefCount;   //���ü���
public:
	Resource(ResourceType type, std::string globalName);
	~Resource();
	void AddRef() {
		++m_intRefCount;
	}
	void Release();
	
};