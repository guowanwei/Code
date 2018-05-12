#pragma once
#include "../BaseUtilities/AS3DVector.h"
#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <vector>
class RenderTarget;


class DrawQuad
{
protected:
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


public:
	DrawQuad(const WCHAR* ShaderFilePath);
	virtual ~DrawQuad();
	virtual void Render(std::vector<RenderTarget*>& InputRenderTargets, std::vector<RenderTarget*>& OutputRenderTargets)=0;
};