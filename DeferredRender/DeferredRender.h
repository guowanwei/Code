#pragma once
#include "../IRender.h"
#include "../RenderTarget.h"
#include "../PostProcess/Quad.h"
class DeferredRender : public IRender
{
	RenderTarget* Normal;
	RenderTarget* BaseColor;
	RenderTarget* RoughnessMetallic;
	RenderTarget* WorldPos;

	DrawQuad* Quad;
public:
	virtual void Render() override;
private:
	DeferredRender();
public:
	static DeferredRender& Instance()
	{
		static DeferredRender DRender;
		return DRender;
	}
	~DeferredRender();
};
