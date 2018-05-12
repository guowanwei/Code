#include "Quad.h"
#include "ToneMap.h"
#include "../Device.h"

struct TonemapPixelConBuffer
{
	AS3DVECTOR4 adapted_lum;  //������Ļ�ռ��ƽ������
};
PostProcessTonemap::PostProcessTonemap(const WCHAR* ShaderFilePath)
	:DrawQuad(ShaderFilePath)
{
	//create const buffer
	D3D11_BUFFER_DESC matrixBufferDescPS;
	ZeroMemory(&matrixBufferDescPS, sizeof(matrixBufferDescPS));
	matrixBufferDescPS.Usage = D3D11_USAGE_DEFAULT;
	matrixBufferDescPS.ByteWidth = sizeof(TonemapPixelConBuffer);
	matrixBufferDescPS.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDescPS.CPUAccessFlags = 0;
	Device::Instance().GetDevice()->CreateBuffer(&matrixBufferDescPS, NULL, &mCBMatrixBufferPS);
}

PostProcessTonemap::~PostProcessTonemap()
{
	if (mCBMatrixBufferPS)
	{
		mCBMatrixBufferPS->Release();
		mCBMatrixBufferPS = NULL;
	}
}
void PostProcessTonemap::Render(std::vector<RenderTarget*>& InputRenderTargets, std::vector<RenderTarget*>& OutputRenderTargets)
{

	//������Ҫ���㣬������Ļ��ƽ������
	//r * 0.299 + g * 0.587 + b * 0.114
	TonemapPixelConBuffer cbufferPixel;
	cbufferPixel.adapted_lum = AS3DVECTOR4(0.5f, 0.0f, 0.0f, 0.0f);
	Device::Instance().GetContext()->UpdateSubresource(mCBMatrixBufferPS, 0, NULL, &cbufferPixel, 0, 0);
	//
	Device::Instance().GetContext()->PSSetConstantBuffers(0, 1, &mCBMatrixBufferPS);
	DrawQuad::Render(InputRenderTargets, OutputRenderTargets);
}