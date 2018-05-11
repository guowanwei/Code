
#include "ForwardRender.h"
#include "../Device.h"
#include "../WorldManager.h"
void ForwardRender::Render()
{
	//set render target
	Device::Instance().DrawOnScreenFinally();
	Device::Instance().ClearRenderTarget();
	Device::Instance().ClearDepthBuffer();

	//draw all objects 
	WorldManager::Instance().RenderOpaqueObjects();

	//draw transparent objects
	WorldManager::Instance().RenderTransparentObjects();
}