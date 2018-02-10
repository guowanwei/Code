#include "stdafx.h"
#include "Object.h"
#include "BaseUtilities/AS3DMatrix.h"
Object::Object(WorldTransform transform)
	:bVisiable(true)
{
	bUseIBL = false;
	this->transform = transform;
}
AS3DMATRIX4 Object::GetWorldTransformMatrix()
{
	//先缩放，在旋转，最后平移
	AS3DMATRIX4 result;
	result.Identity();
	AS3DMATRIX4 mat;
	mat.Scale(transform.scale.x, transform.scale.y, transform.scale.z);
	result *= mat;
	result *= TransformMatrix(transform.dir, transform.up, transform.location);
	mat.Translate(transform.location.x, transform.location.y, transform.location.z);
	result *= mat;
	return result;
}
Object::~Object()
{

}