#pragma once
#include "BaseUtilities/AS3DVector.h"
#include<d3d11.h>
#include<d3dx11.h>
struct QuardVertexFormat
{
	AS3DVECTOR3 mPos;
	AS3DVECTOR2 mUV;
};
class RenderQuard
{
	ID3D11Buffer * mVertexBuffer;
	ID3D11Buffer * mCBMatrixBufferVS;
	ID3D11Buffer * mCBMatrixBufferPS;


	//shaders
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;

	ID3D11PixelShader* mGBufferPixelShader;

	//inputlayout
	ID3D11InputLayout* mInputLayout;

	//Rasterize State
	ID3D11RasterizerState* mRasterizerState;

	//Blend State
	ID3D11BlendState* mBlendState;

	ID3D11DepthStencilState* mDepthStencilState;

private:
	RenderQuard() {}
public:
	static RenderQuard& Instance()
	{
		static RenderQuard renderQuard;
		return renderQuard;
	}

	~RenderQuard()
	{
		if (mVertexBuffer) mVertexBuffer->Release();
		mVertexBuffer = 0;

		if (mCBMatrixBufferVS) mCBMatrixBufferVS->Release();
		mCBMatrixBufferVS = 0;

		if (mCBMatrixBufferPS) mCBMatrixBufferPS->Release();
		mCBMatrixBufferPS = 0;

		if (mVertexShader) mVertexShader->Release();
		mVertexShader = 0;

		if (mPixelShader) mPixelShader->Release();
		mPixelShader = 0;
		if (mInputLayout) mInputLayout->Release();
		mInputLayout = 0;

		if (mGBufferPixelShader) mGBufferPixelShader->Release();
		mGBufferPixelShader = 0;

		if (mRasterizerState) mRasterizerState->Release();
		mRasterizerState = 0;

		if (mBlendState) mBlendState->Release();
		mBlendState = 0;
		
		if (mDepthStencilState) mDepthStencilState->Release();
		mDepthStencilState = 0;
	}
};
