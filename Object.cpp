#include "stdafx.h"
#include "Object.h"
Object::Object(WorldTransform transform)
	:bVisiable(true)
{
	bUseIBL = false;
	this->transform = transform;
}
XMMATRIX Object::GetWorldTransformMatrix()
{
	//�����ţ�����ת�����ƽ��
	return  XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z)* TransformMatrix(transform.dir, transform.up, transform.location) * XMMatrixTranslation(transform.location.x, transform.location.y, transform.location.z);
}
Object::~Object()
{

}