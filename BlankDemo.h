#ifndef _BLANK_DEMO_H_
#define _BLANK_DEMO_H_
#include"DemoBase.h"
class BlankDemo : public Dx11DemoBase
{
public:
	BlankDemo( );
	virtual ~BlankDemo( );
	bool LoadContent( );
	void UnloadContent( );
	void Update( float dt );
	void Render( );
private:
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	float mTheta;
	float mPhi;
	float mRadius;
};
#endif