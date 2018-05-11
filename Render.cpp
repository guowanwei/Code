#include "stdafx.h"
#include "Render.h"
#include "Device.h"
#include "WorldManager.h"
#include "count_performance.h"
#include "DeferredRender/DeferredRender.h"
#include "ForwardRender/ForwardRender.h"
Render& Render::Instance()
{
	static Render render;
	return render;
}

Render::Render()
{
}
Render::~Render()
{
}
void Render::RenderFlow()
{
	//forward rendering
	if (!WorldManager::Instance().IsDeferringRenderMode())
	{
		ForwardRender::Instance().Render();
	}
	else // deferred rendering
	{
		DeferredRender::Instance().Render();
	}

	BeginCountPerformance(Present)
	//BeginCountPerformance(ggg)
	Device::Instance().GetSwapChain()->Present(0, 0);
	EndCountPerformance(Present)

}