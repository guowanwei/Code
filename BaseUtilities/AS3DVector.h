#pragma once
#include<math.h>
///////////////////////////////////////////////////////////////////////////
//
//	class AS3DVECTOR3
//
///////////////////////////////////////////////////////////////////////////
inline bool a_Equals(float a, float b, float eps)
{
	const float diff = fabs(a - b);
	return (diff < eps) ? true : false;
}

class AS3DVECTOR3
{
public:		//	Types

public:		//	Constructions and Destructions

	AS3DVECTOR3()
	{
	}
	explicit AS3DVECTOR3(float m) : x(m), y(m), z(m)
	{
	}
	AS3DVECTOR3(float x, float y, float z) : x(x), y(y), z(z)
	{
	}
	AS3DVECTOR3(const AS3DVECTOR3& v) : x(v.x), y(v.y), z(v.z) { }

public:		//	Attributes
	union
	{
		struct
		{
			float x, y, z;
		};

		float m[3];
	};

public:		//	Operaitons

	//	+ operator
	friend AS3DVECTOR3 operator + (const AS3DVECTOR3& v1, const AS3DVECTOR3& v2)
	{
		return AS3DVECTOR3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

	//	- operator
	friend AS3DVECTOR3 operator - (const AS3DVECTOR3& v1, const AS3DVECTOR3& v2)
	{
		return AS3DVECTOR3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	//	* operator
	friend AS3DVECTOR3 operator * (const AS3DVECTOR3& v, float f)
	{
		return AS3DVECTOR3(v.x*f, v.y*f, v.z*f);
	}

	friend AS3DVECTOR3 operator * (float f, const AS3DVECTOR3& v)
	{
		return AS3DVECTOR3(v.x*f, v.y*f, v.z*f);
	}
	friend AS3DVECTOR3 operator * (const AS3DVECTOR3& v, int f) { return v * (float)f; }
	friend AS3DVECTOR3 operator * (int f, const AS3DVECTOR3& v) { return v * (float)f; }
	friend AS3DVECTOR3 operator * (const AS3DVECTOR3& v1, const AS3DVECTOR3& v2)
	{
		return AS3DVECTOR3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
	}

	//	/ operator
	friend AS3DVECTOR3 operator / (const AS3DVECTOR3& v, float f)
	{
		f = 1.0f / f;
		return AS3DVECTOR3(v.x*f, v.y*f, v.z*f);
	}

	friend AS3DVECTOR3 operator / (const AS3DVECTOR3& v, int f) { return v / (float)f; }

	friend AS3DVECTOR3 operator / (const AS3DVECTOR3& v1, const AS3DVECTOR3& v2)
	{
		return AS3DVECTOR3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
	}

	//	Dot product
	friend float DotProduct(const AS3DVECTOR3& v1, const AS3DVECTOR3& v2);

	//	Cross product
	friend AS3DVECTOR3 CrossProduct(const AS3DVECTOR3& v1, const AS3DVECTOR3& v2)
	{
			return AS3DVECTOR3(v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x);
	}
	void CrossProduct(const AS3DVECTOR3& v1, const AS3DVECTOR3& v2)
	{
		x = v1.y * v2.z - v1.z * v2.y;
		y = v1.z * v2.x - v1.x * v2.z;
		z = v1.x * v2.y - v1.y * v2.x;
	}

	//	== operator
	friend bool operator == (const AS3DVECTOR3& v1, const AS3DVECTOR3& v2) { return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z; }
	//	!= operator
	friend bool operator != (const AS3DVECTOR3& v1, const AS3DVECTOR3& v2) { return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z; }

	//	+= operator
	const AS3DVECTOR3& operator += (const AS3DVECTOR3& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	//	-= operator
	const AS3DVECTOR3& operator -= (const AS3DVECTOR3& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	//	*= operator
	const AS3DVECTOR3& operator *= (float f)
	{
		x *= f; y *= f; z *= f;
		return *this;
	}

	//	/= operator
	const AS3DVECTOR3& operator /= (float f)
	{
		f = 1.0f / f;
		x *= f; y *= f; z *= f;
		return *this;
	}

	//	= operator
	const AS3DVECTOR3& operator = (const AS3DVECTOR3& v)
	{
		x = v.x; y = v.y; z = v.z;
		return *this;
	}

	AS3DVECTOR3 operator + () const { return *this; }
	AS3DVECTOR3 operator - () const { return AS3DVECTOR3(-x, -y, -z); }

	void Set(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	void Clear()
	{
		x = y = z = 0.0f;
	}
	bool IsZero() const { return x == 0.0f && y == 0.0f && z == 0.0f; }
	bool IsEquals(const AS3DVECTOR3 & v, float epsilon) const
	{
		return a_Equals(x, v.x, epsilon) && a_Equals(y, v.y, epsilon) && a_Equals(z, v.z, epsilon);
	}

	//	Magnitude
	float Magnitude() const
	{
		return (float)(sqrt(x * x + y * y + z * z));
	}

	float MagnitudeH() const { return (float)(sqrt(x * x + z * z)); }

	float SquaredMagnitude() const
	{
		float fRet;
		fRet = x * x + y * y + z * z;
		return fRet;
	}

	float SquaredMagnitudeH() const { return x * x + z * z; }

	//	Normalize
	float Normalize()
	{
		float fMag = Magnitude();
		if (fMag < 1e-6	&& fMag > -1e-6)
		{
			Clear();
			fMag = 0.0f;
		}
		else
		{
			
			float f = 1.0f / fMag;
			x = x * f;
			y = y * f;
			z = z * f;
		}
		return fMag;
	}

	//	Get normalize
	friend float Normalize(const AS3DVECTOR3& vIn, AS3DVECTOR3& vOut)
	{
		float fMag = vIn.Magnitude();
		if (fMag < 1e-6	&& fMag > -1e-6)
		{
			vOut.Clear();
			fMag = 0.0f;
		}
		else
		{
			float f = 1.0f / fMag;
			vOut = vIn * f;
		}

		return fMag;
	}

	// LERP method.
	friend AS3DVECTOR3 LERPVec(const AS3DVECTOR3& v1, const AS3DVECTOR3& v2, float fraction)
	{
		return v1 * (1.0f - fraction) + v2 * fraction;
	}

	//	Get minimum number
	float MinMember() const
	{
		if (x < y)
			return x < z ? x : z;
		else
			return y < z ? y : z;
	}

	//	Get maximum member
	float MaxMember() const
	{
		if (x > y)
			return x > z ? x : z;
		else
			return y > z ? y : z;
	}

	//	Snap vector
	void Snap()
	{
		for (int i = 0; i < 3; i++)
		{
			if (m[i] > 1.0f - 1e-5f)
			{
				Clear();
				m[i] = 1.0f;
				break;
			}
			else if (m[i] < -1.0f + 1e-5f)
			{
				Clear();
				m[i] = -1.0f;
				break;
			}
		}
	}

protected:	//	Attributes

protected:	//	Operations

};

inline float DotProduct(const AS3DVECTOR3& v1, const AS3DVECTOR3& v2)
{
	float fRet;
	fRet = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	return fRet;
}

///////////////////////////////////////////////////////////////////////////
//
//	class AS3DVECTOR4
//
///////////////////////////////////////////////////////////////////////////

class AS3DVECTOR4 //: public AMemBase
{
public:		//	Attributes

	union
	{
		struct
		{
			float x, y, z, w;
		};

		float m[4];
	};

public:		//	Constructors and Destructors

	AS3DVECTOR4()
	{
	}
	AS3DVECTOR4(float _x, float _y, float _z, float _w)
	{
		x = _x; y = _y; z = _z; w = _w;
	}
	explicit AS3DVECTOR4(float f)
	{
		x = y = z = w = f;
	}

	AS3DVECTOR4(const float f[4])
	{
		x = f[0]; y = f[1]; z = f[2]; w = f[3];
	}

	AS3DVECTOR4(const AS3DVECTOR4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	//AS3DVECTOR4(const AS3DVECTOR4& v) : x(v.x), y(v.y), z(v.z), w(1.0f) {}

public:		//	Operations

			//	= operator
	const AS3DVECTOR4& operator = (const AS3DVECTOR4& v)
	{
		x = v.x; y = v.y; z = v.z; w = v.w;
		return *this;
	}

	const AS3DVECTOR4& operator = (const AS3DVECTOR3& v)
	{
		x = v.x; y = v.y; z = v.z; w = 1.0f;
		return *this;
	}

	void Set(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }

	void Clear()
	{
		x = y = z = w = 0.0f;
	}

	bool IsEquals(const AS3DVECTOR4 & v, float epsilon) const
	{
		return a_Equals(x, v.x, epsilon) && a_Equals(y, v.y, epsilon) && a_Equals(z, v.z, epsilon) && a_Equals(w, v.w, epsilon);
	}
};

///////////////////////////////////////////////////////////////////////////
//
//	class AS3DVECTOR2
//
///////////////////////////////////////////////////////////////////////////

class AS3DVECTOR2 //: public AMemBase
{
public:		//	Types

public:		//	Constructions and Destructions

	explicit AS3DVECTOR2()
	{
	}

	AS3DVECTOR2(float m) : x(m), y(m)
	{
	}
	AS3DVECTOR2(float x, float y) : x(x), y(y)
	{
	}
	AS3DVECTOR2(const AS3DVECTOR2& v) : x(v.x), y(v.y) {}

public:		//	Attributes

	union
	{
		struct
		{
			float x, y;
		};

		float m[2];
	};

public:		//	Operaitons

			//	+ operator
	friend AS3DVECTOR2 operator + (const AS3DVECTOR2& v1, const AS3DVECTOR2& v2)
	{
		return AS3DVECTOR2(v1.x + v2.x, v1.y + v2.y);
	}
	//	- operator
	friend AS3DVECTOR2 operator - (const AS3DVECTOR2& v1, const AS3DVECTOR2& v2)
	{
		return AS3DVECTOR2(v1.x - v2.x, v1.y - v2.y);
	}

	//	* operator
	friend AS3DVECTOR2 operator * (const AS3DVECTOR2& v, float f)
	{
		return AS3DVECTOR2(v.x*f, v.y*f);
	}

	friend AS3DVECTOR2 operator * (float f, const AS3DVECTOR2& v)
	{
		return AS3DVECTOR2(v.x*f, v.y*f);
	}


	friend AS3DVECTOR2 operator * (const AS3DVECTOR2& v, int f) { return v * (float)f; }

	friend AS3DVECTOR2 operator * (int f, const AS3DVECTOR2& v) { return v * (float)f; }

	friend AS3DVECTOR2 operator * (const AS3DVECTOR2& v1, const AS3DVECTOR2& v2)
	{
		return AS3DVECTOR2(v1.x*v2.x, v1.y*v2.y);
	}

	//	/ operator
	friend AS3DVECTOR2 operator / (const AS3DVECTOR2& v, float f)
	{
		f = 1.0f / f;
		return AS3DVECTOR2(v.x*f, v.y*f);
	}

	friend AS3DVECTOR2 operator / (const AS3DVECTOR2& v, int f) { return v / (float)f; }
	friend AS3DVECTOR2 operator / (const AS3DVECTOR2& v1, const AS3DVECTOR2& v2)
	{
		return AS3DVECTOR2(v1.x / v2.x, v1.y / v2.y);
	}

	//	Dot product
	friend float DotProduct(const AS3DVECTOR2& v1, const AS3DVECTOR2& v2);

	//	== operator
	friend bool operator == (const AS3DVECTOR2& v1, const AS3DVECTOR2& v2) { return v1.x == v2.x && v1.y == v2.y; }
	//	!= operator
	friend bool operator != (const AS3DVECTOR2& v1, const AS3DVECTOR2& v2) { return v1.x != v2.x || v1.y != v2.y; }

	//	+= operator
	const AS3DVECTOR2& operator += (const AS3DVECTOR2& v)
	{
		x += v.x; y += v.y;
		return *this;
	}

	//	-= operator
	const AS3DVECTOR2& operator -= (const AS3DVECTOR2& v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}

	//	*= operator
	const AS3DVECTOR2& operator *= (float f)
	{
		x *= f; y *= f;
		return *this;
	}

	//	/= operator
	const AS3DVECTOR2& operator /= (float f)
	{
		f = 1.0f / f;
		x *= f; y *= f;
		return *this;
	}

	//	= operator
	const AS3DVECTOR2& operator = (const AS3DVECTOR2& v)
	{
		x = v.x; y = v.y;
		return *this;
	}

	AS3DVECTOR2 operator + () const { return *this; }
	AS3DVECTOR2 operator - () const { return AS3DVECTOR2(-x, -y); }

	void Set(float _x, float _y) { x = _x; y = _y; }

	void Clear()
	{
		x = y = 0.0f;
	}

	bool IsZero() const { return x == 0.0f && y == 0.0f; }
	bool IsEquals(const AS3DVECTOR2 & v, float epsilon) const
	{
		return a_Equals(x, v.x, epsilon) && a_Equals(y, v.y, epsilon);
	}

	//	Magnitude
	float Magnitude() const
	{
		float fRet;
		fRet = (float)(sqrt(x * x + y * y));
		return fRet;
	}

	float SquaredMagnitude() const
	{
		float fRet;
		fRet = x * x + y * y;
		return fRet;
	}

	//	Normalize
	float Normalize()
	{
		float fMag = Magnitude();
		if (fMag < 1e-6	&& fMag > -1e-6)
		{
			Clear();
			fMag = 0.0f;
		}
		else
		{
			float f = 1.0f / fMag;
			x = x * f;
			y = y * f;
		}
		return fMag;
	}

	//	Get normalize
	friend float Normalize(const AS3DVECTOR2& vIn, AS3DVECTOR2& vOut)
	{
		float fMag = vIn.Magnitude();
		if (fMag < 1e-6	&& fMag > -1e-6)
		{
			vOut.Clear();
			fMag = 0.0f;
		}
		else
		{
			float f = 1.0f / fMag;
			vOut = vIn * f;
		}

		return fMag;
	}

	// LERP method.
	friend AS3DVECTOR2 LERPVec(const AS3DVECTOR2& v1, const AS3DVECTOR2& v2, float fraction)
	{
		return v1 * (1.0f - fraction) + v2 * fraction;
	}

	//	Get minimum number
	float MinMember() const
	{
		if (x < y)
			return x;
		else
			return y;
	}

	//	Get maximum member
	float MaxMember() const
	{
		if (x > y)
			return x;
		else
			return y;
	}

	//	Snap vector
	void Snap()
	{
		for (int i = 0; i < 2; i++)
		{
			if (m[i] > 1.0f - 1e-5f)
			{
				Clear();
				m[i] = 1.0f;
				break;
			}
			else if (m[i] < -1.0f + 1e-5f)
			{
				Clear();
				m[i] = -1.0f;
				break;
			}
		}
	}

protected:	//	Attributes

protected:	//	Operations

};

inline float DotProduct(const AS3DVECTOR2& v1, const AS3DVECTOR2& v2)
{
	float fRet;
	fRet = v1.x*v2.x + v1.y*v2.y;
	return fRet;
}

// in case ambiguous call, treat as vector3
inline float DotProduct(float v1, float v2) { return v1 * v2; }



