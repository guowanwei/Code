#include "../stdafx.h"
#include "Actor.h"
#include "../Resource/Material.h"	
#include "../Resource/MaterialInstance.h"
#include "../Resource/Mesh.h"
Actor::Actor()
{

}
Actor::~Actor()
{
	if (m_ucMesh) m_ucMesh->Release();
	m_ucMesh = NULL;

	if (m_ucMaterial) m_ucMaterial->Release();
	m_ucMaterial = NULL;
	
	if (m_ucMaterialIns) m_ucMaterialIns->Release();
	m_ucMaterialIns = NULL;
}