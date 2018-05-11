#include "stdafx.h"
#include "Render.h"
#include "Device.h"
#include "WorldManager.h"
#include "count_performance.h"
#include "DeferredRender/DeferredRender.h"
Render& Render::Instance()
{
	static Render render;
	return render;
}

Render::Render()
{
	UINT ScreenWidth = Device::Instance().GetWinWidth();
	UINT ScreenHeight = Device::Instance().GetWinHeight();
	ToneMapRenderTarget = new RenderTarget(ScreenWidth, ScreenHeight, DXGI_FORMAT_R16G16B16A16_FLOAT);
}
Render::~Render()
{
	if (ToneMapRenderTarget) delete ToneMapRenderTarget;
	ToneMapRenderTarget = 0;
}
void Render::RenderFlow()
{
	//现将world渲染到ToneMapRenderTarget中
	/*
	ToneMapRenderTarget->ApplyToDevice();
	ToneMapRenderTarget->ClearRenderTarget();
	WorldManager::Instance().render();
	*/

	//最后渲染的屏幕中来
	//forward rendering
	if (!WorldManager::Instance().IsDeferringRenderMode())
	{
		Device::Instance().DrawOnScreenFinally();
		Device::Instance().ClearRenderTarget();
		WorldManager::Instance().render();
		//WorldManager::Instance().GenGBuffer();
	}
	else
	{
		Device::Instance().ClearRenderTarget();
		DeferredRender::Instance().Render();
	}

	BeginCountPerformance(Present)
	//BeginCountPerformance(ggg)
	Device::Instance().GetSwapChain()->Present(0, 0);
	EndCountPerformance(Present)

}