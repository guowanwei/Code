#pragma once
#include<string.h>
#include "AS3DVector.h"
///////////////////////////////////////////////////////////////////////////
//
//	class A3DMATRIX3
//
///////////////////////////////////////////////////////////////////////////

class AS3DMATRIX3 //: public AMemBase
{
public:		//	Types

			//	Construct flag
	enum CONSTRUCT
	{
		IDENTITY = 0,	//	Construct a identity matrix
		CLEARED,		//	Construct a cleared matrix
	};

public:		//	Constructions and Destructions

	AS3DMATRIX3()
	{
	}

	AS3DMATRIX3(CONSTRUCT c)
	{
		if (c == IDENTITY)
		{
			_12 = _13 = 0.0f;
			_21 = _23 = 0.0f;
			_31 = _32 = 0.0f;
			_11 = _22 = _33 = 1.0f;
		}
		else if (c == CLEARED)
		{
			_11 = _12 = _13 = 0.0f;
			_21 = _22 = _23 = 0.0f;
			_31 = _32 = _33 = 0.0f;
		}
	}

	AS3DMATRIX3(const float arr[3][3])
	{
		memcpy(m, arr, 9 * sizeof(float));
	}

	AS3DMATRIX3(const AS3DMATRIX3& rkMatrix)
	{
		memcpy(m, rkMatrix.m, 9 * sizeof(float));
	}

	AS3DMATRIX3(float fEntry00, float fEntry01, float fEntry02,
		float fEntry10, float fEntry11, float fEntry12,
		float fEntry20, float fEntry21, float fEntry22)
	{
		m[0][0] = fEntry00;
		m[0][1] = fEntry01;
		m[0][2] = fEntry02;
		m[1][0] = fEntry10;
		m[1][1] = fEntry11;
		m[1][2] = fEntry12;
		m[2][0] = fEntry20;
		m[2][1] = fEntry21;
		m[2][2] = fEntry22;
	}

public:		//	Attributes

	union
	{
		struct
		{
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};

		float m[3][3];
	};

public:		//	Operations

	AS3DVECTOR3 GetRow(int i) const { return AS3DVECTOR3(m[i][0], m[i][1], m[i][2]); }
	AS3DVECTOR3 GetCol(int i) const { return AS3DVECTOR3(m[0][i], m[1][i], m[2][i]); }

	//	* operator
	friend AS3DVECTOR3 operator * (const AS3DVECTOR3& v, const AS3DMATRIX3& mat);
	friend AS3DVECTOR3 operator * (const AS3DMATRIX3& mat, const AS3DVECTOR3& v);
	friend AS3DMATRIX3 operator * (const AS3DMATRIX3& mat1, const AS3DMATRIX3& mat2);

	//	*= operator
	AS3DMATRIX3& operator *= (const AS3DMATRIX3& mat)
	{
		*this = *this * mat;
		return *this;
	}

	void Transpose()
	{
		float t;
		t = m[0][1]; m[0][1] = m[1][0]; m[1][0] = t;
		t = m[0][2]; m[0][2] = m[2][0];	m[2][0] = t;
		t = m[1][2]; m[1][2] = m[2][1]; m[2][1] = t;
	}

	//	Clear all elements to zero
	void Clear();
	//	Set matrix to identity matrix
	void Identity();

	//	Build matrix to be translation and rotation matrix
	void Translate(float x, float y);
	void Rotate(float fRad);

	void InverseTM();

};

///////////////////////////////////////////////////////////////////////////
//
//	class AS3DMATRIX4
//
///////////////////////////////////////////////////////////////////////////

class AS3DMATRIX4 //: public AMemBase
{
public:		//	Types

			//	Construct flag
	enum CONSTRUCT
	{
		IDENTITY = 0,	//	Construct a identity matrix
	};

public:		//	Constructions and Destructions

	AS3DMATRIX4()
	{
	}

	AS3DMATRIX4(float* v)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				m[i][j] = v[i * 4 + j];
		}
	}
	AS3DMATRIX4(const AS3DMATRIX4& mat)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				m[i][j] = mat.m[i][j];
		}
	}
	AS3DMATRIX4(CONSTRUCT c)
	{
		_12 = _13 = _14 = 0.0f;
		_21 = _23 = _24 = 0.0f;
		_31 = _32 = _34 = 0.0f;
		_41 = _42 = _43 = 0.0f;
		_11 = _22 = _33 = _44 = 1.0f;
	}
	AS3DMATRIX4(float fEntry00, float fEntry01, float fEntry02, float fEntry03,
		float fEntry10, float fEntry11, float fEntry12, float fEntry13,
		float fEntry20, float fEntry21, float fEntry22, float fEntry23,
		float fEntry30, float fEntry31, float fEntry32, float fEntry33)
	{
		m[0][0] = fEntry00;
		m[0][1] = fEntry01;
		m[0][2] = fEntry02;
		m[0][3] = fEntry03;

		m[1][0] = fEntry10;
		m[1][1] = fEntry11;
		m[1][2] = fEntry12;
		m[1][3] = fEntry13;

		m[2][0] = fEntry20;
		m[2][1] = fEntry21;
		m[2][2] = fEntry22;
		m[2][3] = fEntry23;

		m[3][0] = fEntry30;
		m[3][1] = fEntry31;
		m[3][2] = fEntry32;
		m[3][3] = fEntry33;
	}

public:		//	Attributes

	union
	{
		struct
		{
			float	_11, _12, _13, _14;
			float	_21, _22, _23, _24;
			float	_31, _32, _33, _34;
			float	_41, _42, _43, _44;
		};

		float m[4][4];
	};

public:		//	Operaitons

			//	Get row and column
	AS3DVECTOR3 GetRow(int i) const { return AS3DVECTOR3(m[i][0], m[i][1], m[i][2]); }
	AS3DVECTOR3 GetCol(int i) const { return AS3DVECTOR3(m[0][i], m[1][i], m[2][i]); }
	//	Set row and column
	void SetRow(int i, const AS3DVECTOR3& v) { m[i][0] = v.x; m[i][1] = v.y; m[i][2] = v.z; }
	void SetCol(int i, const AS3DVECTOR3& v) { m[0][i] = v.x; m[1][i] = v.y; m[2][i] = v.z; }

	//	* operator

	friend AS3DVECTOR4 operator * (const AS3DVECTOR4& v, const AS3DMATRIX4& mat);
	friend AS3DVECTOR4 operator * (const AS3DMATRIX4& mat, const AS3DVECTOR4& v);
	friend AS3DMATRIX4 operator * (const AS3DMATRIX4& mat1, const AS3DMATRIX4& mat2);

	//	Scale matrix
	friend AS3DMATRIX4 operator * (const AS3DMATRIX4& mat, float s);
	friend AS3DMATRIX4 operator * (float s, const AS3DMATRIX4& mat) { return mat * s; }
	friend AS3DMATRIX4 operator / (const AS3DMATRIX4& mat, float s) { return mat * (1.0f / s); }

	//	*= operator
	AS3DMATRIX4& operator *= (const AS3DMATRIX4& mat)
	{
		*this = *this * mat;
		return *this;
	}
	AS3DMATRIX4& operator *= (float s);
	//	/= operator
	AS3DMATRIX4& operator /= (float s) { return *this *= (1.0f / s); }

	//	== operator
	friend bool operator == (const AS3DMATRIX4& mat1, const AS3DMATRIX4& mat2)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (mat1.m[i][j] != mat2.m[i][j])
					return false;
			}
		}
		return true;
	}

	//	!= operator
	friend bool operator != (const AS3DMATRIX4& mat1, const AS3DMATRIX4& mat2)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (mat1.m[i][j] != mat2.m[i][j])
					return true;
			}
		}
		return false;
	}

	//	+ operator
	friend AS3DMATRIX4 operator + (const AS3DMATRIX4& mat1, const AS3DMATRIX4& mat2);
	//	- operator
	friend AS3DMATRIX4 operator - (const AS3DMATRIX4& mat1, const AS3DMATRIX4& mat2);
	//	+= operator
	AS3DMATRIX4& operator += (const AS3DMATRIX4& mat);
	//	-= operator
	AS3DMATRIX4& operator -= (const AS3DMATRIX4& mat);

	//	Clear all elements to zero
	void Clear();
	//	Set matrix to identity matrix
	void Identity();
	//	Transpose matrix
	void Transpose();
	//	Get transpose matrix of this matrix
	AS3DMATRIX4 GetTranspose() const;
	//	Inverse matrix
	void Inverse() { *this = GetInverse(); }
	//	Get inverse matrix of this matrix
	AS3DMATRIX4 GetInverse() const;
	//	Inverse matrix (used only by transform matrix)
	void InverseTM() { *this = GetInverseTM(); }
	//	Get inverse matrix (used only by transform matrix)
	AS3DMATRIX4 GetInverseTM() const;
	//	Get determinant of this matrix
	float Determinant() const;

	//	Build matrix to be translation and rotation matrix
	void Translate(float x, float y, float z);
	void RotateX(float fRad);
	void RotateY(float fRad);
	void RotateZ(float fRad);
	void RotateAxis(const AS3DVECTOR3& v, float fRad);
	void RotateAxis(const AS3DVECTOR3& vPos, const AS3DVECTOR3& vAxis, float fRad);
	void Scale(float sx, float sy, float sz);

	//	Set/Get rotation part of transform matrix
	void SetRotatePart(const AS3DMATRIX3& mat);
	AS3DMATRIX3 GetRotatePart() const;
	//	Set/Get translate part of transform matrix
	void SetTransPart(const AS3DVECTOR3& vTrans) { SetRow(3, vTrans); }
	AS3DVECTOR3 GetTransPart() const { return GetRow(3); };

protected:	//	Attributes

			//	Calcualte determinant of a 3x3 matrix
	float Det3x3(float a11, float a12, float a13, float a21, float a22, float a23,
		float a31, float a32, float a33) const
	{
		return a11 * a22 * a33 + a21 * a32 * a13 + a31 * a12 * a23 -
			a13 * a22 * a31 - a23 * a32 * a11 - a33 * a12 * a21;
	}

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

AS3DMATRIX4 operator * (const AS3DMATRIX4& mat1, const AS3DMATRIX4& mat2);