#pragma once
#include "Model.h"
class RainDrop : public Model
{
	float CurScale;
public:
	RainDrop(const char* FbxFilePath, const WCHAR* ShaderFilePath, WorldTransform transform);
	virtual bool init() override;
	virtual void update(float Delta) override;
	virtual void render() override;
	//virtual void recompileshader() override;
	virtual ~RainDrop();
	void Play()
	{
		if (bVisiable) return;
		bVisiable = true;
		CurScale = 0;
	}
};