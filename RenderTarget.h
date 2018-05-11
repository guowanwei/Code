#pragma once
#include<d3d11.h>
#include<d3dx11.h>
class RenderTarget
{
	ID3D11Texture2D * mRenderTargetBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11ShaderResourceView* mShaderResourceView;
public:
	UINT mWidth;
	UINT mHeight;
	DXGI_FORMAT mFormat;
public:
	RenderTarget(UINT width, UINT height, DXGI_FORMAT RenderTargetFormat);
	~RenderTarget();


	ID3D11ShaderResourceView*& GetShaderResourceView();
	ID3D11RenderTargetView*& GetRenderTargetView();
	
	bool ClearRenderTarget();
};
