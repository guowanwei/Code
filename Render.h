#pragma once
#include "RenderTarget.h"
class Render
{
private:
	Render();
public:
	static Render& Instance();
	~Render();
	void RenderFlow();
};
