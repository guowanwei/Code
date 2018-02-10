#pragma once
#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>
#include "BaseUtilities.h"
#include "Device.h"
#include "BaseUtilities/AS3DVector.h"
struct WorldTransform
{
	AS3DVECTOR3 location; //world space position
	AS3DVECTOR3 dir;    // direction; forward
	AS3DVECTOR3 up;     // upward
	AS3DVECTOR3 scale;   // scale factor
	WorldTransform()
		:location(0.0f,0.0f,0.0f)
		, dir(0.0f,0.0f,1.0f)
		, up(0.0f,1.0f,0.0f)
		,scale(1.0f,1.0f,1.0f)
	{

	}
	WorldTransform(AS3DVECTOR3 loc, AS3DVECTOR3 dir, AS3DVECTOR3 up, AS3DVECTOR3 scale)
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
	AS3DMATRIX4 GetWorldTransformMatrix();
	void SetPosition(AS3DVECTOR3 loc) { transform.location = loc; }
	void SetScale(AS3DVECTOR3 Scale) { transform.scale = Scale; }
};