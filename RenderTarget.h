#pragma once
#include<d3d11.h>
#include<d3dx11.h>
class RenderTarget
{
	ID3D11Texture2D * mDepthStencilBuffer;
	ID3D11Texture2D * mRenderTargetBuffer;

	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11ShaderResourceView* mShaderResourceView;
	ID3D11DepthStencilView* mDepthStencilView;
public:
	RenderTarget(UINT width, UINT height, DXGI_FORMAT RenderTargetFormat);
	bool ApplyAsTexture(int layer);
	bool ApplyToDevice();
	~RenderTarget();
	bool ClearRenderTarget();
};
