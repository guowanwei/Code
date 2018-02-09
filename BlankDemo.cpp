#include "StdAfx.h"
#include"BlankDemo.h"
const float PI = 3.1415f;
BlankDemo::BlankDemo( )
{
	mTheta = 1.5f * PI;
	mPhi = 0.25f * PI;
	mRadius = 5.0f;
	
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);
}
BlankDemo::~BlankDemo( )
{}
bool BlankDemo::LoadContent( )
{
	return Dx11DemoBase::LoadContent();
}
void BlankDemo::UnloadContent( )
{}
void BlankDemo::Update( float dt )
{
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(10, 0, 0, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*PI, 1.0f, 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);

	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX worldViewProj = XMMatrixTranspose(world*view*proj);

	d3dContext_->UpdateSubresource(mCBMatrixBuffer, 0, NULL, &worldViewProj, 0, 0);
}
void BlankDemo::Render( )
{
	if( d3dContext_ == 0 )
		return;
	//清空rendertarget
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );
	//清空depthbuffer
	d3dContext_->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//这里是drawcall 正式内容！



	//设置顶点输入布局
	d3dContext_->IASetInputLayout(mInputLayout);

	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3dContext_->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	d3dContext_->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//d3dContext_->CSSetConstantBuffers(0,1,)
	d3dContext_->VSSetConstantBuffers(0, 1, &mCBMatrixBuffer);
	//设置VertexShader和PixelShader
	d3dContext_->VSSetShader(mVertexShader, NULL, 0);
	d3dContext_->PSSetShader(mPixelShader, NULL, 0);


	//设置采样状态
	//d3dContext_->PSSetSamplers(0, 1, &mSamplerState);  //S0注册 对应0
												 //渲染三角形
	d3dContext_->DrawIndexed(36, 0, 0);

	swapChain_->Present( 0, 0 );
}