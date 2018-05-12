#include "DeferredRender.h"
#include "../RenderTargetManager.h"
#include "../Device.h"
#include "../WorldManager.h"
#include "DeferredShadingStage.h"
#include "../PostProcess/ToneMap.h"
void DeferredRender::Render()
{
	//set render target and depth buffer
	Device& device  = Device::Instance();
	BaseColor = RenderTargetManager::Instance().PopRenderTarget(device.GetWinWidth(), device.GetWinHeight(), DXGI_FORMAT_R8G8B8A8_UNORM);
	device.ClearRenderTarget(BaseColor->GetRenderTargetView());
	Normal = RenderTargetManager::Instance().PopRenderTarget(device.GetWinWidth(),device.GetWinHeight(), DXGI_FORMAT_R8G8B8A8_UNORM);
	device.ClearRenderTarget(Normal->GetRenderTargetView());
	RoughnessMetallic = RenderTargetManager::Instance().PopRenderTarget(device.GetWinWidth(), device.GetWinHeight(), DXGI_FORMAT_R8G8B8A8_UNORM);
	device.ClearRenderTarget(RoughnessMetallic->GetRenderTargetView());
	WorldPos = RenderTargetManager::Instance().PopRenderTarget(device.GetWinWidth(), device.GetWinHeight(), DXGI_FORMAT_R16G16B16A16_FLOAT);
	device.ClearRenderTarget(WorldPos->GetRenderTargetView());

	std::vector<RenderTarget*> GBufferRenderTargets;
	GBufferRenderTargets.push_back(BaseColor);
	GBufferRenderTargets.push_back(Normal);
	GBufferRenderTargets.push_back(RoughnessMetallic);
	GBufferRenderTargets.push_back(WorldPos);
	device.SetRenderTargets(GBufferRenderTargets, NULL);
	device.ClearDepthBuffer();

	
	// GBuffer stage
	WorldManager::Instance().GenGBuffer();


	//set final render target
	device.DrawOnScreenFinally();
	device.ClearRenderTarget();

	//light stage
	std::vector<RenderTarget*> LightResultRenderTargets;
	RenderTarget* LightResult = RenderTargetManager::Instance().PopRenderTarget(device.GetWinWidth(), device.GetWinHeight(), DXGI_FORMAT_R16G16B16A16_FLOAT);
	LightResultRenderTargets.push_back(LightResult);
	Quad->Render(GBufferRenderTargets, LightResultRenderTargets);

	

	//render transparent objects
	//
	WorldManager::Instance().RenderTransparentObjects();


	//tonemap
	std::vector<RenderTarget*> TonemapResult;
	PostProcessTonemap::Instance().Render(LightResultRenderTargets, TonemapResult);



	//back rendertarget
	RenderTargetManager::Instance().PushRenderTarget(Normal);
	Normal = NULL;
	RenderTargetManager::Instance().PushRenderTarget(BaseColor);
	BaseColor = NULL;
	RenderTargetManager::Instance().PushRenderTarget(RoughnessMetallic);
	RoughnessMetallic = NULL;
	RenderTargetManager::Instance().PushRenderTarget(WorldPos);
	WorldPos = NULL;
	RenderTargetManager::Instance().PushRenderTarget(LightResult);
	LightResult = NULL;
	
}

DeferredRender::DeferredRender()
{
	Quad = new DeferredShading(L"Resource/Shader/Lighting.hlsl");
}
DeferredRender::~DeferredRender()
{
	if (Quad) delete Quad;
	Quad = NULL;
}