#pragma once
#include "RenderTarget.h"
class Render
{
private:
	Render();

	RenderTarget* ToneMapRenderTarget;
public:
	static Render& Instance();
	~Render();
	void RenderFlow();
};
