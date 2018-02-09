#pragma once
#include "Object.h"
#include <vector>
class ConeRain;
class RainDrop;
class WorldManager
{
private:
	std::vector<Object*> Objects;
	//
	ConeRain* pRain;
	RainDrop* pRainDrop;
	WorldManager() { pRain = NULL; };
public:
	void ModifyRainIntensify(bool add);
	void PlayRainDrop();
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
	}
};