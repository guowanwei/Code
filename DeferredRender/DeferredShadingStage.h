#pragma once
#include <vector>
class DrawQuad;

class DeferredShading:public DrawQuad
{
	ID3D11Buffer * mCBMatrixBufferPS;
public:
	DeferredShading(const WCHAR* ShaderFilePath);
	~DeferredShading();
	virtual void Render(std::vector<RenderTarget*>& InputRenderTargets,std::vector<RenderTarget*>& OutputRenderTargets) override;
};