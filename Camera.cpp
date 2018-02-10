#include "stdafx.h"
#include"Camera.h"
#include "BaseUtilities.h"
Camera::Camera()
{

}
Camera::Camera(AS3DVECTOR3 Position, AS3DVECTOR3 Right, AS3DVECTOR3 Up, AS3DVECTOR3 Dir, float NearZ, float FarZ, float Aspect, float FovY)
	:mPosition(Position), mRight(Right), mUp(Up), mDir(Dir)
	, mNearZ(NearZ), mFarZ(FarZ), mAspect(Aspect), mFovY(FovY)
	, mDistance(30)
{

}
Camera::Camera(AS3DVECTOR3 pos, AS3DVECTOR3 target, AS3DVECTOR3 worldUp, float NearZ, float FarZ, float Aspect, float FovY)
	:mNearZ(NearZ), mFarZ(FarZ), mAspect(Aspect), mFovY(FovY)
	, mDistance(30)
{
	LookAt(pos, target, worldUp);
}
void Camera::Init(AS3DVECTOR3 pos, AS3DVECTOR3 target, AS3DVECTOR3 worldUp, float NearZ, float FarZ, float Aspect, float FovY)
{
	mNearZ = NearZ;
	mFarZ = FarZ;
	mAspect = Aspect;
	mFovY = FovY;
	LookAt(pos, target, worldUp);
	mDistance = 30.0f;
}
void Camera::LookAt(AS3DVECTOR3 pos, AS3DVECTOR3 target, AS3DVECTOR3 worldUp)
{
	mDir = target - pos;
	mDir.Normalize();

	mRight.CrossProduct(worldUp, mDir);
	mRight.Normalize();

	mUp.CrossProduct(mDir, mRight);
	mUp.Normalize();

	mPosition = pos;
}
void Camera::UpdateDirAndUp()
{
	//
	float Length =sqrt(mDir.x * mDir.x + mDir.y * mDir.y + mDir.z * mDir.z);
	AS3DVECTOR3 TargetPoint;
	TargetPoint.x = mPosition.x + mDir.x / Length*mDistance;
	TargetPoint.y = mPosition.y + mDir.y / Length*mDistance;
	TargetPoint.z = mPosition.z + mDir.z / Length*mDistance;

	mDir.x = (float)(cos(mDeg) * cos(mPitch));
	mDir.y = (float)sin(mPitch);
	mDir.z = (float)(-sin(mDeg) * cos(mPitch));
}
AS3DMATRIX4 Camera::GetView()
{

	// Keep camera's axes orthogonal to each other and of unit length.
	mDir.Normalize();
	mUp.CrossProduct(mDir, mRight);
	mUp.Normalize();
	
	// U, L already ortho-normal, so no need to normalize cross product.
	mRight.CrossProduct(mUp, mDir);

	// Fill in the view matrix entries.
	float x = -DotProduct(mPosition, mRight);
	float y = -DotProduct(mPosition, mUp);
	float z = -DotProduct(mPosition, mDir);

	
	 return AS3DMATRIX4(mRight.x, mUp.x, mDir.x, 0.0f,
		mRight.y, mUp.y, mDir.y, 0.0f,
		mRight.z, mUp.z, mDir.z, 0.0f,
		x,y,z,1.0f
			);
}
AS3DMATRIX4 Camera::GetProj()
{
	return AS3DPerspectiveFovMatrix(mFovY, mAspect, mNearZ, mFarZ);
}
AS3DMATRIX4 Camera::GetViewProj()
{
	return GetView() * GetProj();
}

Camera& Camera::Instance()
{
	static Camera camera;
	return camera;
}
AS3DVECTOR3 Camera::GetEyePos()
{
	return mPosition;
}