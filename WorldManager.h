#pragma once
#include "Object.h"
#include <vector>
#include "ComputeShaderTestModule/ComputeShaderTestModule.h"
class ConeRain;
class RainDrop;
class WorldManager
{
private:
	std::vector<Object*> Objects;

	//
	ComputeShaderTestModule * pComputeShaderTest;
	ConeRain* pRain;
	RainDrop* pRainDrop;
	WorldManager() {
		pRain = NULL; 
		DeferredRendering = false;
	};
	bool DeferredRendering;
public:
	void ModifyRainIntensify(bool add);
	void PlayRainDrop();
	void SwitchRenderMode();
	bool IsDeferringRenderMode();
	static WorldManager& Instance() {
		static WorldManager worldManager;
		return worldManager;
	}
	void init();
	void update(float delta);
	void render();
	void SwitchShaderMode();
	void AddObject(Object* object);
	void RemoveObject(Object* object)
	{
	};
	~WorldManager() 
	{
		for (int i = 0; i < Objects.size(); ++i)
		{
			delete Objects[i];
		}
		if (pComputeShaderTest)
		{
			delete pComputeShaderTest;
			pComputeShaderTest = NULL;
		}
	}

	//
	void GenGBuffer();
	void RenderOpaqueObjects();
	void RenderTransparentObjects();
};