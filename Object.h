#pragma once
#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#include "BaseUtilities.h"
#include "Device.h"

struct WorldTransform
{
	XMFLOAT3 location; //world space position
	XMFLOAT3 dir;    // direction; forward
	XMFLOAT3 up;     // upward
	XMFLOAT3 scale;   // scale factor
	WorldTransform()
		:location(0.0f,0.0f,0.0f)
		, dir(0.0f,0.0f,1.0f)
		, up(0.0f,1.0f,0.0f)
		,scale(1.0f,1.0f,1.0f)
	{

	}
	WorldTransform(XMFLOAT3 loc, XMFLOAT3 dir, XMFLOAT3 up, XMFLOAT3 scale)
		:location(loc)
		,dir(dir)
		,up(up)
		,scale(scale)
	{

	}
};
class Object
{
private:
	WorldTransform transform;
protected:
	bool bUseIBL;
	bool bVisiable;
public:
	void HideObject() { bVisiable = false; }
	void ShowObject() { bVisiable = true; }
	Object(WorldTransform transform);
	virtual bool init() = 0;
	virtual void update(float Delta) = 0;
	virtual void render()  = 0;
	virtual void recompileshader() = 0;
	virtual ~Object();
	XMMATRIX GetWorldTransformMatrix();
	void SetPosition(XMFLOAT3 loc) { transform.location = loc; }
	void SetScale(XMFLOAT3 Scale) { transform.scale = Scale; }
};