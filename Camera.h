#pragma once
#include "BaseUtilities/AS3DVector.h"
#include "BaseUtilities/AS3DMatrix.h"
class Camera
{

	AS3DVECTOR3 mPosition;
	AS3DVECTOR3 mRight;
	AS3DVECTOR3 mUp;
	AS3DVECTOR3 mDir;


	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;

	float mDistance;

private:
	Camera(AS3DVECTOR3 Position, AS3DVECTOR3 Right, AS3DVECTOR3 Up, AS3DVECTOR3 Dir, float NearZ, float FarZ, float Aspect, float FovY);
	Camera(AS3DVECTOR3 pos, AS3DVECTOR3 target, AS3DVECTOR3 worldUp, float NearZ, float FarZ, float Aspect, float FovY);
	Camera();
public:
	static Camera& Instance();
	void Init(AS3DVECTOR3 pos, AS3DVECTOR3 target, AS3DVECTOR3 worldUp, float NearZ, float FarZ, float Aspect, float FovY);
	void LookAt(AS3DVECTOR3 pos, AS3DVECTOR3 target, AS3DVECTOR3 worldUp);
	AS3DMATRIX4 GetView();
	AS3DMATRIX4 GetProj();
	AS3DMATRIX4 GetViewProj();

	AS3DVECTOR3 GetEyePos();
	AS3DVECTOR3 GetDir() { return mDir; }
	AS3DVECTOR3 GetRight() { return mRight; }


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