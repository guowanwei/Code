#pragma once

#include<xnamath.h>
#include<vector>
#include<map>

extern const float PI;

std::wstring string_to_wstring(std::string str);
std::string wstring_to_string(std::wstring str);
XMFLOAT3 Normalize(const XMFLOAT3& v);
XMFLOAT3 CrossProduct(const XMFLOAT3& v1, const XMFLOAT3& v2);
XMMATRIX TransformMatrix(const XMFLOAT3& dir, const XMFLOAT3& up,const XMFLOAT3& pos);
struct BaseCommonVertex
{
	XMFLOAT3 mPosition;
	XMFLOAT3 mNormal;
	XMFLOAT3 mTangent;
	XMFLOAT3 mBinormal;

	XMFLOAT2 mUV;
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