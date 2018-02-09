#pragma once
#include "Object.h"
#include <vector>
class Terrain : public Object
{
public:
	struct InitInfo
	{
		std::wstring HeightMapFilename;
		std::wstring LayerMapFilename0;
		std::wstring LayerMapFilename1;
		std::wstring LayerMapFilename2;
		std::wstring LayerMapFilename3;
		std::wstring LayerMapFilename4;
		std::wstring BlendMapFilename;
		float HeightScale;
		UINT HeightmapWidth;
		UINT HeightmapHeight;
		float CellSpacing;
	};

	void LoadHeightmap();
	Terrain(WorldTransform transform);
	virtual bool init()override;
	virtual void update(float Delta)override;
	virtual void render()override;
	virtual ~Terrain();
	void recompileshader() {}
private:
	InitInfo mInfo;
	std::vector<float> mHeightmap;
};