#include "stdafx.h"
#include <locale>
#include <codecvt>
#include <string>
#include "BaseUtilities.h"
std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
std::wstring string_to_wstring(std::string str) { return converter.from_bytes(str); }
std::string wstring_to_string(std::wstring str) { return converter.to_bytes(str); }
XMFLOAT3 Normalize(const XMFLOAT3& v)
{
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length > -1.0e-5 && length < 1.0e-5)
		return XMFLOAT3(0, 0, 0);
	return XMFLOAT3(v.x/length,v.y/length,v.z/length);
}
XMFLOAT3 CrossProduct(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return XMFLOAT3(v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
}
XMMATRIX TransformMatrix(const XMFLOAT3& dir, const XMFLOAT3& up, const XMFLOAT3& pos)
{
	XMFLOAT3 vecXAxis, vecYAxis, vecZAxis;
	vecZAxis = Normalize(dir);
	vecYAxis = Normalize(up);
	vecXAxis = Normalize(CrossProduct(vecYAxis, vecZAxis));

	XMMATRIX mat;
	memset(&mat, 0, sizeof(mat));
	mat.m[0][0] = vecXAxis.x;
	mat.m[0][1] = vecXAxis.y;
	mat.m[0][2] = vecXAxis.z;

	mat.m[1][0] = vecYAxis.x;
	mat.m[1][1] = vecYAxis.y;
	mat.m[1][2] = vecYAxis.z;

	mat.m[2][0] = vecZAxis.x;
	mat.m[2][1] = vecZAxis.y;
	mat.m[2][2] = vecZAxis.z;

	/*
	mat.m[3][0] = pos.x;
	mat.m[3][1] = pos.y;
	mat.m[3][2] = pos.z;
	*/
	mat.m[3][3] = 1.0f;

	return mat;
}
const float PI = 3.1415926f;