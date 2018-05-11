#include "DeferredRender.h"
#include "../RenderTargetManager.h"
#include "../Device.h"
#include "../WorldManager.h"
void DeferredRender::Render()
{
	//set render target
	Device& device  = Device::Instance();
	BaseColor = RenderTargetManager::Instance().PopRenderTarget(device.GetWinWidth(), device.GetWinHeight(), DXGI_FORMAT_R8G8B8A8_UNORM);
	device.ClearRenderTarget(BaseColor->GetRenderTargetView());
	Normal = RenderTargetManager::Instance().PopRenderTarget(device.GetWinWidth(),device.GetWinHeight(), DXGI_FORMAT_R8G8B8A8_UNORM);
	device.ClearRenderTarget(Normal->GetRenderTargetView());
	RoughnessMetallic = RenderTargetManager::Instance().PopRenderTarget(device.GetWinWidth(), device.GetWinHeight(), DXGI_FORMAT_R8G8B8A8_UNORM);
	device.ClearRenderTarget(RoughnessMetallic->GetRenderTargetView());
	WorldPos = RenderTargetManager::Instance().PopRenderTarget(device.GetWinWidth(), device.GetWinHeight(), DXGI_FORMAT_R16G16B16A16_FLOAT);
	device.ClearRenderTarget(WorldPos->GetRenderTargetView());
	RenderTarget* tmp[4] = { BaseColor ,Normal,RoughnessMetallic,WorldPos };
	device.SetRenderTargets(4, &tmp[0], NULL);
	
	// GBuffer stage
	WorldManager::Instance().GenGBuffer();



	device.DrawOnScreenFinally();
	Device::Instance().ClearRenderTarget();
	//light stage
	Quad->render(BaseColor, Normal, RoughnessMetallic, WorldPos);

	//�黹rendertarget
	RenderTargetManager::Instance().PushRenderTarget(Normal);
	Normal = NULL;
	RenderTargetManager::Instance().PushRenderTarget(BaseColor);
	BaseColor = NULL;
	RenderTargetManager::Instance().PushRenderTarget(RoughnessMetallic);
	RoughnessMetallic = NULL;
	RenderTargetManager::Instance().PushRenderTarget(WorldPos);
	WorldPos = NULL;

}

DeferredRender::DeferredRender()
{
	Quad = new DrawQuad();
}
DeferredRender::~DeferredRender()
{
	if (Quad) delete Quad;
	Quad = NULL;
}