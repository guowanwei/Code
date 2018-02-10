#include "stdafx.h"
#include <locale>
#include <codecvt>
#include <string>
#include "BaseUtilities.h"

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
std::wstring string_to_wstring(std::string str) { return converter.from_bytes(str); }
std::string wstring_to_string(std::wstring str) { return converter.to_bytes(str); }
AS3DMATRIX4 TransformMatrix(const AS3DVECTOR3& dir, const AS3DVECTOR3& up, const AS3DVECTOR3& pos)
{
	AS3DVECTOR3 vecXAxis, vecYAxis, vecZAxis;
	vecZAxis = dir; vecZAxis.Normalize();
	vecYAxis = up; vecYAxis.Normalize();
	vecXAxis = CrossProduct(vecYAxis, vecZAxis);
	vecXAxis.Normalize();

	AS3DMATRIX4 mat;
	mat.Clear();
	mat.m[0][0] = vecXAxis.x;
	mat.m[0][1] = vecXAxis.y;
	mat.m[0][2] = vecXAxis.z;

	mat.m[1][0] = vecYAxis.x;
	mat.m[1][1] = vecYAxis.y;
	mat.m[1][2] = vecYAxis.z;

	mat.m[2][0] = vecZAxis.x;
	mat.m[2][1] = vecZAxis.y;
	mat.m[2][2] = vecZAxis.z;

	mat.m[3][3] = 1.0f;

	return mat;
}
const float PI = 3.1415926f;

AS3DMATRIX4 AS3DPerspectiveFovMatrix(float fFovY, float fAspect, float fZNear, float fZFar)
{
	AS3DMATRIX4 m;
	float fYScale = 1.0f / tanf(fFovY / 2.0f);
	float fXScale = fYScale / fAspect;
	m._11 = fXScale;
	m._22 = fYScale;
	m._33 = fZFar / (fZFar - fZNear);
	m._43 = -fZNear * m._33;
	m._34 = 1.0f;
	m._12 = m._13 = m._14 = 0.0f;
	m._21 = m._23 = m._24 = 0.0f;
	m._31 = m._32 = 0.0f;
	m._41 = m._42 = m._44 = 0.0f;
	return m;
}