#include "stdafx.h"
#include"Camera.h"
Camera::Camera()
{

}
Camera::Camera(XMFLOAT3 Position, XMFLOAT3 Right, XMFLOAT3 Up, XMFLOAT3 Dir, float NearZ, float FarZ, float Aspect, float FovY)
	:mPosition(Position), mRight(Right), mUp(Up), mDir(Dir)
	, mNearZ(NearZ), mFarZ(FarZ), mAspect(Aspect), mFovY(FovY)
	, mDistance(30)
{

}
Camera::Camera(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp, float NearZ, float FarZ, float Aspect, float FovY)
	:mNearZ(NearZ), mFarZ(FarZ), mAspect(Aspect), mFovY(FovY)
	, mDistance(30)
{
	LookAt(pos, target, worldUp);
}
void Camera::Init(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp, float NearZ, float FarZ, float Aspect, float FovY)
{
	mNearZ = NearZ;
	mFarZ = FarZ;
	mAspect = Aspect;
	mFovY = FovY;
	LookAt(pos, target, worldUp);
	mDistance = 30.0f;
}
void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Normalize(XMVector3Cross(L, R));

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mDir, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
}
void Camera::UpdateDirAndUp()
{
	//
	float Length =sqrt(mDir.x * mDir.x + mDir.y * mDir.y + mDir.z * mDir.z);
	XMFLOAT3 TargetPoint;
	TargetPoint.x = mPosition.x + mDir.x / Length*mDistance;
	TargetPoint.y = mPosition.y + mDir.y / Length*mDistance;
	TargetPoint.z = mPosition.z + mDir.z / Length*mDistance;

	mDir.x = (float)(cos(mDeg) * cos(mPitch));
	mDir.y = (float)sin(mPitch);
	mDir.z = (float)(-sin(mDeg) * cos(mPitch));
}
XMMATRIX Camera::GetView()
{
	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR L = XMLoadFloat3(&mDir);
	XMVECTOR P = XMLoadFloat3(&mPosition);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mDir, L);
	XMFLOAT4X4 mView;
	mView(0, 0) = mRight.x;
	mView(1, 0) = mRight.y;
	mView(2, 0) = mRight.z;
	mView(3, 0) = x;

	mView(0, 1) = mUp.x;
	mView(1, 1) = mUp.y;
	mView(2, 1) = mUp.z;
	mView(3, 1) = y;

	mView(0, 2) = mDir.x;
	mView(1, 2) = mDir.y;
	mView(2, 2) = mDir.z;
	mView(3, 2) = z;

	mView(0, 3) = 0.0f;
	mView(1, 3) = 0.0f;
	mView(2, 3) = 0.0f;
	mView(3, 3) = 1.0f;
	return XMLoadFloat4x4(&mView);
}
XMMATRIX Camera::GetProj()
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	return P;
}
XMMATRIX Camera::GetViewProj()
{
	return XMMatrixMultiply(GetView(), GetProj());
}

Camera& Camera::Instance()
{
	static Camera camera;
	return camera;
}
XMFLOAT3 Camera::GetEyePos()
{
	return mPosition;
}