#pragma once
#include "BaseUtilities/AS3DVector.h"
#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>
class RenderTarget;
struct QuadPixelConBuffer
{
	AS3DVECTOR4 LightDir;
	AS3DVECTOR4 LightColor;
	AS3DVECTOR4 EyePosW;
};
struct QuadVertexData
{
	AS3DVECTOR3 Pos;
	AS3DVECTOR3 UV;
	QuadVertexData(AS3DVECTOR3 P, AS3DVECTOR3 U)
		:Pos(P),UV(U)
	{
	}
};
class DrawQuad
{
	ID3D11Buffer * mVertexBuffer;
	//inputlayout
	ID3D11InputLayout* mInputLayout;
	//shaders
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;

	ID3D11SamplerState* mSamplerState;

	
	ID3D11RasterizerState* mRasterizerState;
	ID3D11BlendState* mBlendState;
	ID3D11DepthStencilState* mDepthStencilState;

	ID3D11Buffer * mCBMatrixBufferPS;
public:
	DrawQuad();
	~DrawQuad();
	void render(RenderTarget* BaseColor, RenderTarget* Normal, RenderTarget* RoughnessMetallic, RenderTarget* WorldPos);
};