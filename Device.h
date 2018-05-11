#pragma once
#include<d3d11.h>
#include<d3dx11.h>

class RenderTarget;
class Device
{
private:
	UINT mWinWidth;
	UINT mWinHeight;
	Device() {};
	D3D_DRIVER_TYPE driverType_;
	D3D_FEATURE_LEVEL featureLevel_;

	ID3D11Device* d3dDevice_;
	ID3D11DeviceContext* d3dContext_;
	IDXGISwapChain* swapChain_;
	ID3D11RenderTargetView* backBufferTarget_;

	ID3D11Texture2D * mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;

public:
	UINT GetWinWidth() {
		return mWinWidth;
	}
	UINT GetWinHeight() {
		return mWinHeight;
	}
	bool init(HWND hwnd);
	static Device& Instance()
	{
		static Device device;
		return device;
	}
	ID3D11Device* GetDevice()
	{
		return d3dDevice_;
	}
	ID3D11DeviceContext* GetContext()
	{
		return d3dContext_;
	}
	IDXGISwapChain* GetSwapChain()
	{
		return swapChain_;
	}
	void ClearRenderTarget()
	{
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
		//d3dContext_->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	}
	void ClearRenderTarget(ID3D11RenderTargetView* RenderTargetView)
	{
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		d3dContext_->ClearRenderTargetView(RenderTargetView, clearColor);
	}

	void ClearDepthBuffer()
	{
		d3dContext_->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	~Device();
	bool DrawOnScreenFinally();

	bool SetRenderTargets(int TargetsNum, RenderTarget** RenderTargets, ID3D11DepthStencilView* DepthBuffer = NULL);
};