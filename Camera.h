#pragma once
#include<xnamath.h>
class Camera
{

	XMFLOAT3 mPosition;
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mDir;


	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;

	float mDistance;

private:
	Camera(XMFLOAT3 Position, XMFLOAT3 Right, XMFLOAT3 Up, XMFLOAT3 Dir, float NearZ, float FarZ, float Aspect, float FovY);
	Camera(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp, float NearZ, float FarZ, float Aspect, float FovY);
	Camera();
public:
	static Camera& Instance();
	void Init(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp, float NearZ, float FarZ, float Aspect, float FovY);
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	XMMATRIX GetView();
	XMMATRIX GetProj();
	XMMATRIX GetViewProj();

	XMFLOAT3 GetEyePos();
	XMFLOAT3 GetDir() { return mDir; }
	XMFLOAT3 GetRight() { return mRight; }


private:
	float mDeg;
	float mPitch;
	void UpdateDirAndUp();
public:
	void DegDelta(float fYaw)
	{
		mDeg += fYaw;
		UpdateDirAndUp();
	}
	void PitchDelta(float fPitch)
	{
		mPitch += fPitch;
		UpdateDirAndUp();
	}
};