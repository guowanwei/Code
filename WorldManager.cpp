#include "stdafx.h"
#include "WorldManager.h"
#include "Device.h"
#include "BaseUtilities.h"
#include "Model.h"
#include "Sky.h"
#include "ConeRain.h"
#include "RainDrop.h"
#include "SphereForTest.h"

void WorldManager::init()
{
	struct ModelLoadInfo
	{
		char* ModelPath;
		WCHAR* ShaderPath;
		WorldTransform transform;
	};
	ModelLoadInfo ModelPath[] =
	{
		{ "Resource/Model/CMC_MF_01.FBX",L"Resource/Shader/Common.hlsl",{ AS3DVECTOR3(2000.0f,0.0f,0.0f),AS3DVECTOR3(0.0f,0,1.0f),AS3DVECTOR3(0,1,0),AS3DVECTOR3(1.0f,1.0f,1.0f)}},//房子
		{ "Resource/Model/CMC_CM.FBX",L"Resource/Shader/Common.hlsl",{ AS3DVECTOR3(0.0f,0.0f,0.0f),AS3DVECTOR3(0.0f,0,1.0f),AS3DVECTOR3(0,1,0),AS3DVECTOR3(1.0f,1.0f,1.0f) }},//主要房子
		{ "Resource/Model/ChaSi.FBX",L"Resource/Shader/Common.hlsl",{ AS3DVECTOR3(-2000.0f,0.0f,0.0f),AS3DVECTOR3(0.0f,1,0.0f),AS3DVECTOR3(0,0,1),AS3DVECTOR3(1.0f,1.0f,1.0f) } },//茶肆
	};
	// models
	for (int i = 0; i < sizeof(ModelPath) / sizeof(ModelPath[0]); ++i)
	{
		Object* pObject = new Model(ModelPath[i].ModelPath, ModelPath[i].ShaderPath, ModelPath[i].transform);
		pObject->init();
		Objects.push_back(pObject);
	}
	
	//sky 
	{
		AS3DVECTOR3 scale(10.0f, 10.0f, 10.0f);
		AS3DVECTOR3 loc(0, 0.0f, 0);
		AS3DVECTOR3 dir(0.0f, 0.0f, 1.0f);
		AS3DVECTOR3 up(0.0f, 1.0f, 1.0f);
		Object* sky = new Sky(WorldTransform(loc, dir, up, scale));
		sky->init();
		Objects.push_back(sky);
	}
	//rain drop
	ModelLoadInfo RainDropModel[] =
	{
		//{ "Resource/Model/CMC_MF_01.FBX",L"Resource/Shader/Common.hlsl",{ XMFLOAT3(2000.0f,0.0f,0.0f),XMFLOAT3(0.0f,0,1.0f),XMFLOAT3(0,1,0),XMFLOAT3(1.0f,1.0f,1.0f)}},//房子
		//{ "Resource/Model/CMC_CM.FBX",L"Resource/Shader/Common.hlsl",{ XMFLOAT3(0.0f,0.0f,0.0f),XMFLOAT3(0.0f,0,1.0f),XMFLOAT3(0,1,0),XMFLOAT3(1.0f,1.0f,1.0f) }},//主要房子
		//{ "Resource/Model/ChaSi.FBX",L"Resource/Shader/Common.hlsl",{ XMFLOAT3(-2000.0f,0.0f,0.0f),XMFLOAT3(0.0f,1,0.0f),XMFLOAT3(0,0,1),XMFLOAT3(1.0f,1.0f,1.0f) } },//茶肆
		{ "Resource/Model/RainDrop0.FBX",L"Resource/Shader/RainDrop.hlsl",{ AS3DVECTOR3(0.0f,1000.0f,0.0f),AS3DVECTOR3(0.0f,1,0.0f),AS3DVECTOR3(0,0,1),AS3DVECTOR3(1.0f,1.0f,1.0f) } },//茶肆
	};
	for (int i = 0; i < sizeof(RainDropModel) / sizeof(RainDropModel[0]); ++i)
	{
		RainDrop* pObject = new RainDrop(RainDropModel[i].ModelPath, RainDropModel[i].ShaderPath, RainDropModel[i].transform);
		pObject->init();
		Objects.push_back(pObject);
		pRainDrop = pObject;
	}
	
	//sphere for test
	/*
	for( int row = 1; row < 9; ++row)
	{
		AS3DVECTOR3 scale(10.0f, 10.0f, 10.0f);
		float x =  1500 + row * 300;
		float z = 2000;
		float roughness = (row + 1) * 0.1;
		{
			AS3DVECTOR3 loc(x, 0.0f, z + 800);
			AS3DVECTOR3 dir(0.0f, 1.0f, 0.0f);
			AS3DVECTOR3 up(0.0f, 0.0f, 1.0f);
			Object* sphereForIBL = new SphereForTest(WorldTransform(loc,dir,up,scale), roughness, AS3DVECTOR3(0.972, 0.960, 0.915));//银子
			sphereForIBL->init();
			Objects.push_back(sphereForIBL);
		}
		{
			AS3DVECTOR3 loc(x, 0.0f, z + 600);
			AS3DVECTOR3 dir(0.0f, 0.0f, 1.0f);
			AS3DVECTOR3 up(0.0f, 1.0f, 0.0f);
			Object* sphereForIBL = new SphereForTest(WorldTransform(loc, dir, up, scale), roughness, AS3DVECTOR3(1.022, 0.782, 0.344)); //金子
			sphereForIBL->init();
			Objects.push_back(sphereForIBL);
		}
		{
			AS3DVECTOR3 loc(x, 0.0f, z + 400);
			AS3DVECTOR3 dir(0.0f, 0.0f, 1.0f);
			AS3DVECTOR3 up(0.0f, 1.0f, 0.0f);
			Object* sphereForIBL = new SphereForTest(WorldTransform(loc, dir, up, scale), roughness, AS3DVECTOR3(0.04, 0.04, 0.04));   //非金属
			sphereForIBL->init();
			Objects.push_back(sphereForIBL);
		}
		{
			AS3DVECTOR3 loc(x, 0.0f, z + 200);
			AS3DVECTOR3 dir(0.0f, 0.0f, 1.0f);
			AS3DVECTOR3 up(0.0f, 1.0f, 0.0f);
			Object* sphereForIBL = new SphereForTest(WorldTransform(loc, dir, up, scale), roughness, AS3DVECTOR3(0.562, 0.565, 0.578));  //铁
			sphereForIBL->init();
			Objects.push_back(sphereForIBL);
		}
	}
	*/
	//conerain
	{
		AS3DVECTOR3 scale(100.0f, 100.0f, 500.0f);
		float x = 2000;
		float z = 2000;
		{
			AS3DVECTOR3 loc(x, 0.0f, z + 800);
			AS3DVECTOR3 dir(0.0f, 1.0f, 0.0f);
			AS3DVECTOR3 up(0.0f, 0.0f, 1.0f);
			Object* conerain = new ConeRain(WorldTransform(loc, dir, up, scale), 0);
			conerain->init();
			pRain = static_cast<ConeRain*>(conerain);
			Objects.push_back(conerain);
		}
	}
	
	//init compute shader test module
	pComputeShaderTest = new ComputeShaderTestModule();
}
void WorldManager::update(float delta)
{
	for (int i = 0; i < Objects.size(); ++i)
		Objects[i]->update(delta);
}
void WorldManager::SwitchShaderMode()
{
	for (int i = 0; i < Objects.size(); ++i)
		Objects[i]->recompileshader();
}
void WorldManager::ModifyRainIntensify(bool add)
{
	if (pRain)
	{
		pRain->AddIntensity(add);
	}
}
void WorldManager::PlayRainDrop() 
{
	if (pRainDrop)
	{
		pRainDrop->Play();
	}
}

void WorldManager::SwitchRenderMode()
{
	DeferredRendering = !DeferredRendering;
}

bool WorldManager::IsDeferringRenderMode()
{
	return DeferredRendering;
}



void WorldManager::GenGBuffer()
{
	
	for (int i = 0; i < Objects.size(); ++i)
	{
		Model* m = dynamic_cast<Model*>(Objects[i]);
		if (m)
			m->GenGBuffer();
	}
}

void WorldManager::RenderOpaqueObjects()
{
	for (int i = 0; i < Objects.size(); ++i)
	{
		Model* m = dynamic_cast<Model*>(Objects[i]);
		if (m)
			m->render();
	}
}

void WorldManager::RenderTransparentObjects()
{
	for (int i = 0; i < Objects.size(); ++i)
	{
		Model* m = dynamic_cast<Model*>(Objects[i]);
		if (!m)
			Objects[i]->render();
	}
}
void WorldManager::render()
{
	/**/
	//Device::Instance().ClearRenderTarget();
	for (int i = 0; i < Objects.size(); ++i)
		Objects[i]->render();
	//Device::Instance().GetSwapChain()->Present(0, 0);


	//这里才是最终的渲染到屏幕上
	/*
	Device::Instance().DrawOnScreenFinally();
	Device::Instance().ClearRenderTarget();

	Device::Instance().GetSwapChain()->Present(0, 0);
	*/

	pComputeShaderTest->Execute();
}
void WorldManager::AddObject(Object* object)
{
	Objects.push_back(object);
}