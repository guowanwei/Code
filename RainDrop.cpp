#include "stdafx.h"
#include "RainDrop.h"
RainDrop::RainDrop(const char* FbxFilePath, const WCHAR* ShaderFilePath, WorldTransform transform)
	:Model(FbxFilePath, ShaderFilePath, transform)
	,CurScale(0)
{
	if (mRasterizerState) mRasterizerState->Release();
	mRasterizerState = NULL;

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.MultisampleEnable = false;
	Device::Instance().GetDevice()->CreateRasterizerState(&rasterDesc, &mRasterizerState);
	
	if (mBlendState) mBlendState->Release();
	mBlendState = NULL;
	// ³õÊ¼»¯»ìºÏ×´Ì¬  
	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;// D3D11_BLEND_SRC_COLOR; // Color_Fsrc  
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;// D3D11_BLEND_DEST_COLOR; // Color_Fdst  
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // Color_Operation  
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fsrc  
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // Alpha_Fdst  
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // Alpha_Operation  
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Device::Instance().GetDevice()->CreateBlendState(
		&blendDesc,
		&mBlendState
	);
}
bool RainDrop::init()
{
	bVisiable = false;
	return Model::init();
}
void RainDrop::update(float Delta)
{
	if (!bVisiable) return;
	CurScale += Delta * 0.004;
	if (CurScale > 1)
	{
		bVisiable = false;
		CurScale = 0.001;
	}

	//
	float scale = CurScale/5;
	AS3DVECTOR3 Scale(scale, scale, scale);
	SetScale(Scale);
	Model::update(Delta);
}
void RainDrop::render()
{
	Model::render();
}
//void recompileshader()
RainDrop::~RainDrop()
{
}