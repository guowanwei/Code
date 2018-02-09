#include "stdafx.h"
#include"RenderTarget.h"
#include "Device.h"
RenderTarget::RenderTarget(UINT width, UINT height, DXGI_FORMAT RenderTargetFormat)
{
	//depth buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	Device::Instance().GetDevice()->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	Device::Instance().GetDevice()->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	D3D11_TEXTURE2D_DESC RenderTargetDesc;
	RenderTargetDesc.Width = width;
	RenderTargetDesc.Height = height;
	RenderTargetDesc.MipLevels = 1;
	RenderTargetDesc.ArraySize = 1;
	RenderTargetDesc.Format = RenderTargetFormat;
	RenderTargetDesc.SampleDesc.Count = 1;
	RenderTargetDesc.SampleDesc.Quality = 0;
	RenderTargetDesc.Usage = D3D11_USAGE_DEFAULT;
	RenderTargetDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	RenderTargetDesc.CPUAccessFlags = 0;
	RenderTargetDesc.MiscFlags = 0;
	Device::Instance().GetDevice()->CreateTexture2D(&RenderTargetDesc, 0, &mRenderTargetBuffer);
	Device::Instance().GetDevice()->CreateRenderTargetView(mRenderTargetBuffer, 0, &mRenderTargetView);
	Device::Instance().GetDevice()->CreateShaderResourceView(mRenderTargetBuffer, 0, &mShaderResourceView);
}
bool RenderTarget::ApplyAsTexture(int layer)
{
	Device::Instance().GetContext()->PSSetShaderResources(layer, 1, &mShaderResourceView);
	return true;
}
bool RenderTarget::ApplyToDevice()
{
	Device::Instance().GetContext()->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	return true;
}
bool RenderTarget::ClearRenderTarget()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Device::Instance().GetContext()->ClearRenderTargetView(mRenderTargetView, clearColor);
	Device::Instance().GetContext()->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	return true;
}

RenderTarget::~RenderTarget()
{
	if (mDepthStencilBuffer) mDepthStencilBuffer->Release();
	mDepthStencilBuffer = 0;
	if (mRenderTargetBuffer) mRenderTargetBuffer->Release();
	mRenderTargetBuffer = 0;
	if (mRenderTargetView) mRenderTargetView->Release();
	mRenderTargetView = 0;
	if (mDepthStencilView) mDepthStencilView->Release();
	mDepthStencilView = 0;
	if (mShaderResourceView) mShaderResourceView->Release();
	mShaderResourceView = 0;
}