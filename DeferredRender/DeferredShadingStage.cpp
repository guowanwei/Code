#include "../PostProcess/Quad.h"
#include "DeferredShadingStage.h"
#include "../Device.h"
#include "../Camera.h"
#include "D3Dcompiler.h"
#include <fstream>
struct QuadPixelConBuffer
{
	AS3DVECTOR4 LightDir;
	AS3DVECTOR4 LightColor;
	AS3DVECTOR4 EyePosW;
};
DeferredShading::DeferredShading(const WCHAR* ShaderFilePath)
	:DrawQuad(ShaderFilePath)
{
	//create const buffer
	D3D11_BUFFER_DESC matrixBufferDescPS;
	ZeroMemory(&matrixBufferDescPS, sizeof(matrixBufferDescPS));
	matrixBufferDescPS.Usage = D3D11_USAGE_DEFAULT;
	matrixBufferDescPS.ByteWidth = sizeof(QuadPixelConBuffer);
	matrixBufferDescPS.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDescPS.CPUAccessFlags = 0;
	Device::Instance().GetDevice()->CreateBuffer(&matrixBufferDescPS, NULL, &mCBMatrixBufferPS);
}

DeferredShading::~DeferredShading()
{
	if (mCBMatrixBufferPS)
	{
		mCBMatrixBufferPS->Release();
		mCBMatrixBufferPS = NULL;
	}
}

void DeferredShading::Render(std::vector<RenderTarget*>& InputRenderTargets,std::vector<RenderTarget*>& OutputRenderTargets)
{
	//¸üÐÂÏñËØconst buffer
	QuadPixelConBuffer cbufferPixel;
	cbufferPixel.LightColor = AS3DVECTOR4(8.0f, 8.0f, 8.0f, 0.0f);
	//cbufferPixel.LightColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	AS3DVECTOR3 tmp1 = Camera::Instance().GetEyePos();
	cbufferPixel.EyePosW.x = tmp1.x;
	cbufferPixel.EyePosW.y = tmp1.y;
	cbufferPixel.EyePosW.z = tmp1.z;
	cbufferPixel.LightDir = AS3DVECTOR4(-1.0f, -1.0f, -1.0f, 0.0f);
	Device::Instance().GetContext()->UpdateSubresource(mCBMatrixBufferPS, 0, NULL, &cbufferPixel, 0, 0);

	Device::Instance().GetContext()->PSSetConstantBuffers(0, 1, &mCBMatrixBufferPS);
	DrawQuad::Render(InputRenderTargets, OutputRenderTargets);
}