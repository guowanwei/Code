#pragma once
#include<vector>
#include<map>
#include "BaseUtilities/AS3DVector.h"
#include "BaseUtilities/AS3DMatrix.h"
extern const float PI;
class AS3DMATRIX4;
std::wstring string_to_wstring(std::string str);
std::string wstring_to_string(std::wstring str);
AS3DMATRIX4 TransformMatrix(const AS3DVECTOR3& dir, const AS3DVECTOR3& up,const AS3DVECTOR3& pos);
struct BaseCommonVertex
{
	AS3DVECTOR3 mPosition;
	AS3DVECTOR3 mNormal;
	AS3DVECTOR3 mTangent;
	AS3DVECTOR3 mBinormal;

	AS3DVECTOR2 mUV;
};
struct MaterialDesc
{
	std::string mDiffuseMapName;
	std::string mNormalMapName;
	std::string RoughnessMetallicMapName;

	UINT startIndex;
	UINT countIndex;
};
struct CommonMeshData
{
	//geometry info
	std::vector<BaseCommonVertex> vertices;
	std::vector<UINT> indices;
	std::map<UINT, MaterialDesc>  materials;
	void Clear()
	{
		vertices.clear();
		indices.clear();
		materials.clear();
	}
};

AS3DMATRIX4 AS3DPerspectiveFovMatrix(float fFovY, float fAspect, float fZNear, float fZFar);