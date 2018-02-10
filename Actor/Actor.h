#pragma once
#include "../BaseUtilities/AS3DVector.h"

class MaterialInstance;
class Material;
class Mesh;
//object in world
class Actor
{
private:

	AS3DVECTOR3 m_v3Dir;  //direction
	AS3DVECTOR3 m_v3Location; //world space position
	AS3DVECTOR3 m_v3Up;     // upward
	AS3DVECTOR3 m_v3Scale;   // scale factor

	//Mesh  user custom type
	Mesh* m_ucMesh;
	// Material uc
	Material* m_ucMaterial;
	//
	MaterialInstance* m_ucMaterialIns;

public:
	Actor();
	~Actor();
};