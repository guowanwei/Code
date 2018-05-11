#include "AS3DMatrix.h"
//	Clear all elements to zero
void AS3DMATRIX3::Clear()
{
	_11 = _12 = _13 = 0.0f;
	_21 = _22 = _23 = 0.0f;
	_31 = _32 = _33 = 0.0f;
}

//	Set matrix to identity matrix
void AS3DMATRIX3::Identity()
{
	_12 = _13 = 0.0f;
	_21 = _23 = 0.0f;
	_31 = _32 = 0.0f;
	_11 = _22 = _33 = 1.0f;
}
//	* operator
AS3DVECTOR3 operator * (const AS3DVECTOR3& v, const AS3DMATRIX3& mat)
{
	return AS3DVECTOR3(v.x * mat._11 + v.y * mat._21 + v.z * mat._31,
		v.x * mat._12 + v.y * mat._22 + v.z * mat._32,
		v.x * mat._13 + v.y * mat._23 + v.z * mat._33);
}

AS3DVECTOR3 operator * (const AS3DMATRIX3& mat, const AS3DVECTOR3& v)
{
	return AS3DVECTOR3(v.x * mat._11 + v.y * mat._21 + v.z * mat._31,
		v.x * mat._12 + v.y * mat._22 + v.z * mat._32,
		v.x * mat._13 + v.y * mat._23 + v.z * mat._33);
}

AS3DMATRIX3 operator * (const AS3DMATRIX3& mat1, const AS3DMATRIX3& mat2)
{
	AS3DMATRIX3 matRet(AS3DMATRIX3::CLEARED);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
				matRet.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
		}
	}
	return matRet;
}


void AS3DMATRIX3::Translate(float x, float y)
{
	Identity();
	_31 = x;
	_32 = y;
}

void AS3DMATRIX3::Rotate(float fRad)
{
	Identity();
	m[1][1] = m[0][0] = (float)cos(fRad);
	m[0][1] = (float)sin(fRad);
	m[1][0] = -m[0][1];
}

void AS3DMATRIX3::InverseTM()
{
	float matInverse[3][3];
	matInverse[0][0] = m[1][1] * m[2][2] -
		m[1][2] * m[2][1];
	matInverse[0][1] = m[0][2] * m[2][1] -
		m[0][1] * m[2][2];
	matInverse[0][2] = m[0][1] * m[1][2] -
		m[0][2] * m[1][1];
	matInverse[1][0] = m[1][2] * m[2][0] -
		m[1][0] * m[2][2];
	matInverse[1][1] = m[0][0] * m[2][2] -
		m[0][2] * m[2][0];
	matInverse[1][2] = m[0][2] * m[1][0] -
		m[0][0] * m[1][2];
	matInverse[2][0] = m[1][0] * m[2][1] -
		m[1][1] * m[2][0];
	matInverse[2][1] = m[0][1] * m[2][0] -
		m[0][0] * m[2][1];
	matInverse[2][2] = m[0][0] * m[1][1] -
		m[0][1] * m[1][0];

	float fDet =
		m[0][0] * matInverse[0][0] +
		m[0][1] * matInverse[1][0] +
		m[0][2] * matInverse[2][0];

	float fInvDet = 1.0f / fDet;
	for (int iRow = 0; iRow < 3; iRow++)
	{
		for (int iCol = 0; iCol < 3; iCol++)
			m[iRow][iCol] = matInverse[iRow][iCol] * fInvDet;
	}
}

//	Matrix4 * vector4
AS3DVECTOR4 operator * (const AS3DVECTOR4& v, const AS3DMATRIX4& mat)
{
	return AS3DVECTOR4(v.x * mat._11 + v.y * mat._21 + v.z * mat._31 + v.w * mat._41,
		v.x * mat._12 + v.y * mat._22 + v.z * mat._32 + v.w * mat._42,
		v.x * mat._13 + v.y * mat._23 + v.z * mat._33 + v.w * mat._43,
		v.x * mat._14 + v.y * mat._24 + v.z * mat._34 + v.w * mat._44);
}

AS3DVECTOR4 operator * (const AS3DMATRIX4& mat, const AS3DVECTOR4& v)
{
	return AS3DVECTOR4(v.x * mat._11 + v.y * mat._21 + v.z * mat._31 + v.w * mat._41,
		v.x * mat._12 + v.y * mat._22 + v.z * mat._32 + v.w * mat._42,
		v.x * mat._13 + v.y * mat._23 + v.z * mat._33 + v.w * mat._43,
		v.x * mat._14 + v.y * mat._24 + v.z * mat._34 + v.w * mat._44);
}


//	Scale matrix4
AS3DMATRIX4 operator * (const AS3DMATRIX4& mat, float s)
{
	AS3DMATRIX4 matRet;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			matRet.m[i][j] = mat.m[i][j] * s;
	}
	return matRet;
}

AS3DMATRIX4& AS3DMATRIX4::operator *= (float s)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			m[i][j] *= s;
	}
	return *this;
}

AS3DMATRIX4 operator + (const AS3DMATRIX4& mat1, const AS3DMATRIX4& mat2)
{
	AS3DMATRIX4 matRet;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			matRet.m[i][j] = mat1.m[i][j] + mat2.m[i][j];
	}
	return matRet;
}

//	- operator
AS3DMATRIX4 operator - (const AS3DMATRIX4& mat1, const AS3DMATRIX4& mat2)
{
	AS3DMATRIX4 matRet;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			matRet.m[i][j] = mat1.m[i][j] - mat2.m[i][j];
	}
	return matRet;
}

//	+= operator
AS3DMATRIX4& AS3DMATRIX4::operator += (const AS3DMATRIX4& mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			m[i][j] += mat.m[i][j];
	}
	return *this;
}

//	-= operator
AS3DMATRIX4& AS3DMATRIX4::operator -= (const AS3DMATRIX4& mat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			m[i][j] -= mat.m[i][j];
	}
	return *this;
}


//	Transpose matrix
void AS3DMATRIX4::Transpose()
{
	float t;
	t = _12; _12 = _21; _21 = t;
	t = _13; _13 = _31; _31 = t;
	t = _14; _14 = _41; _41 = t;
	t = _23; _23 = _32; _32 = t;
	t = _24; _24 = _42; _42 = t;
	t = _34; _34 = _43; _43 = t;
}

//	Get transpose matrix of this matrix
AS3DMATRIX4 AS3DMATRIX4::GetTranspose() const
{
	AS3DMATRIX4 matRet;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			matRet.m[i][j] = m[j][i];
	}
	return matRet;
}

//	Set translation matrix
void AS3DMATRIX4::Translate(float x, float y, float z)
{
	Identity();
	_41 = x;
	_42 = y;
	_43 = z;
}

//	fRad rotate radian
void AS3DMATRIX4::RotateX(float fRad)
{
	Identity();
	m[2][2] = m[1][1] = (float)cos(fRad);
	m[1][2] = (float)sin(fRad);
	m[2][1] = -m[1][2];
}

//	fRad rotate radian
void AS3DMATRIX4::RotateY(float fRad)
{
	Identity();
	m[2][2] = m[0][0] = (float)cos(fRad);
	m[2][0] = (float)sin(fRad);
	m[0][2] = -m[2][0];
}

//	fRad rotate radian
void AS3DMATRIX4::RotateZ(float fRad)
{
	Identity();
	m[1][1] = m[0][0] = (float)cos(fRad);
	m[0][1] = (float)sin(fRad);
	m[1][0] = -m[0][1];
}

void AS3DMATRIX4::RotateAxis(const AS3DVECTOR3& v, float fRad)
{
	float fCos = (float)cos(fRad);
	float fSin = (float)sin(fRad);

	_11 = (v.m[0] * v.m[0]) * (1.0f - fCos) + fCos;
	_21 = (v.m[0] * v.m[1]) * (1.0f - fCos) - (v.m[2] * fSin);
	_31 = (v.m[0] * v.m[2]) * (1.0f - fCos) + (v.m[1] * fSin);

	_12 = (v.m[1] * v.m[0]) * (1.0f - fCos) + (v.m[2] * fSin);
	_22 = (v.m[1] * v.m[1]) * (1.0f - fCos) + fCos;
	_32 = (v.m[1] * v.m[2]) * (1.0f - fCos) - (v.m[0] * fSin);

	_13 = (v.m[2] * v.m[0]) * (1.0f - fCos) - (v.m[1] * fSin);
	_23 = (v.m[2] * v.m[1]) * (1.0f - fCos) + (v.m[0] * fSin);
	_33 = (v.m[2] * v.m[2]) * (1.0f - fCos) + fCos;

	_14 = _24 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_44 = 1.0f;
}

/*	Rotate camera round an axis

vPos: start position of rotate axis
vAxis: rotate axis
fRad: rotate radian
*/
void AS3DMATRIX4::RotateAxis(const AS3DVECTOR3& vPos, const AS3DVECTOR3& vAxis, float fRad)
{
	Translate(-vPos.x, -vPos.y, -vPos.z);

	AS3DMATRIX4 mat;
	mat.RotateAxis(vAxis, fRad);
	*this *= mat;
	mat.Translate(vPos.x, vPos.y, vPos.z);
	*this *= mat;
}

//	Get determinant of this matrix
float AS3DMATRIX4::Determinant() const
{
	float fDet;
	//	按照第一行展开
	fDet = _11 * Det3x3(_22, _23, _24, _32, _33, _34, _42, _43, _44) -
		_12 * Det3x3(_21, _23, _24, _31, _33, _34, _41, _43, _44) +
		_13 * Det3x3(_21, _22, _24, _31, _32, _34, _41, _42, _44) -
		_14 * Det3x3(_21, _22, _23, _31, _32, _33, _41, _42, _43);
	return fDet;
}

//	Get inverse matrix of this matrix
AS3DMATRIX4 AS3DMATRIX4::GetInverse() const
{
	AS3DMATRIX4 mResult;
	float fDet = Determinant();
	mResult._11 = Det3x3(_22, _23, _24, _32, _33, _34, _42, _43, _44);
	mResult._12 = -Det3x3(_12, _13, _14, _32, _33, _34, _42, _43, _44);
	mResult._13 = Det3x3(_12, _13, _14, _22, _23, _24, _42, _43, _44);
	mResult._14 = -Det3x3(_12, _13, _14, _22, _23, _24, _32, _33, _34);

	mResult._21 = -Det3x3(_21, _23, _24, _31, _33, _34, _41, _43, _44);
	mResult._22 = Det3x3(_11, _13, _14, _31, _33, _34, _41, _43, _44);
	mResult._23 = -Det3x3(_11, _13, _14, _21, _23, _24, _41, _43, _44);
	mResult._24 = Det3x3(_11, _13, _14, _21, _23, _24, _31, _33, _34);

	mResult._31 = Det3x3(_21, _22, _24, _31, _32, _34, _41, _42, _44);
	mResult._32 = -Det3x3(_11, _12, _14, _31, _32, _34, _41, _42, _44);
	mResult._33 = Det3x3(_11, _12, _14, _21, _22, _24, _41, _42, _44);
	mResult._34 = -Det3x3(_11, _12, _14, _21, _22, _24, _31, _32, _34);

	mResult._41 = -Det3x3(_21, _22, _23, _31, _32, _33, _41, _42, _43);
	mResult._42 = Det3x3(_11, _12, _13, _31, _32, _33, _41, _42, _43);
	mResult._43 = -Det3x3(_11, _12, _13, _21, _22, _23, _41, _42, _43);
	mResult._44 = Det3x3(_11, _12, _13, _21, _22, _23, _31, _32, _33);

	mResult /= fDet;
	return mResult;
}

//	Get inverse matrix (used only by transform matrix)
AS3DMATRIX4 AS3DMATRIX4::GetInverseTM() const
{
	float fDet = 1.0f / Det3x3(_11, _12, _13, _21, _22, _23, _31, _32, _33);

	AS3DMATRIX4 mat;

	mat._11 = fDet * Det3x3(_22, _23, _24, _32, _33, _34, _42, _43, _44);
	mat._12 = -fDet * Det3x3(_12, _13, _14, _32, _33, _34, _42, _43, _44);
	mat._13 = fDet * Det3x3(_12, _13, _14, _22, _23, _24, _42, _43, _44);
	mat._14 = -fDet * Det3x3(_12, _13, _14, _22, _23, _24, _32, _33, _34);

	mat._21 = -fDet * Det3x3(_21, _23, _24, _31, _33, _34, _41, _43, _44);
	mat._22 = fDet * Det3x3(_11, _13, _14, _31, _33, _34, _41, _43, _44);
	mat._23 = -fDet * Det3x3(_11, _13, _14, _21, _23, _24, _41, _43, _44);
	mat._24 = fDet * Det3x3(_11, _13, _14, _21, _23, _24, _31, _33, _34);

	mat._31 = fDet * Det3x3(_21, _22, _24, _31, _32, _34, _41, _42, _44);
	mat._32 = -fDet * Det3x3(_11, _12, _14, _31, _32, _34, _41, _42, _44);
	mat._33 = fDet * Det3x3(_11, _12, _14, _21, _22, _24, _41, _42, _44);
	mat._34 = -fDet * Det3x3(_11, _12, _14, _21, _22, _24, _31, _32, _34);

	mat._41 = -fDet * Det3x3(_21, _22, _23, _31, _32, _33, _41, _42, _43);
	mat._42 = fDet * Det3x3(_11, _12, _13, _31, _32, _33, _41, _42, _43);
	mat._43 = -fDet * Det3x3(_11, _12, _13, _21, _22, _23, _41, _42, _43);
	mat._44 = fDet * Det3x3(_11, _12, _13, _21, _22, _23, _31, _32, _33);

	return mat;
}

//	Build scale matrix
void AS3DMATRIX4::Scale(float sx, float sy, float sz)
{
	Clear();
	_11 = sx;
	_22 = sy;
	_33 = sz;
	_44 = 1.0f;
}

//	Set rotation part of transform matrix
void AS3DMATRIX4::SetRotatePart(const AS3DMATRIX3& mat)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			m[i][j] = mat.m[i][j];
	}
}

//	Get rotation part of transform matrix
AS3DMATRIX3 AS3DMATRIX4::GetRotatePart() const
{
	AS3DMATRIX3 ret;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			ret.m[i][j] = m[i][j];
	}

	return ret;
}

void AS3DMATRIX4::Clear()
{
	_11 = _12 = _13 = _14 = 0.0f;
	_21 = _22 = _23 = _24 = 0.0f;
	_31 = _32 = _33 = _34 = 0.0f;
	_41 = _42 = _43 = _44 = 0.0f;
}

//	Set matrix to identity matrix
void AS3DMATRIX4::Identity()
{
	_12 = _13 = _14 = 0.0f;
	_21 = _23 = _24 = 0.0f;
	_31 = _32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_11 = _22 = _33 = _44 = 1.0f;
}

AS3DMATRIX4 operator * (const AS3DMATRIX4& mat1, const AS3DMATRIX4& mat2)
{
	AS3DMATRIX4 matRet;
	matRet.Clear();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
				matRet.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
		}
	}
	return matRet;
}