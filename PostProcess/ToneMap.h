#pragma once

#include <vector>
class DrawQuad;
class PostProcessTonemap : public DrawQuad
{
	ID3D11Buffer * mCBMatrixBufferPS;
	PostProcessTonemap(const WCHAR* ShaderFilePath);
public:
	static PostProcessTonemap& Instance()
	{
		static PostProcessTonemap Tonemap(L"Resource/Shader/Tonemap.hlsl");
		return Tonemap;
	}
	~PostProcessTonemap();
	virtual void Render(std::vector<RenderTarget*>& InputRenderTargets, std::vector<RenderTarget*>& OutputRenderTargets) override;
};