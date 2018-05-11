#pragma once
#include "../IRender.h"
#include "../RenderTarget.h"
#include "../Quad.h"
class DeferredRender : public IRender
{
	RenderTarget* Normal;
	RenderTarget* BaseColor;
	RenderTarget* RoughnessMetallic;

	//world position 
	RenderTarget* WorldPos;

	DrawQuad* Quad;
public:
	virtual void Render();
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
