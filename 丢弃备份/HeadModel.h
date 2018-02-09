#pragma once
#include "Object.h"
#include "BaseUtilities.h"
class HeadModel : public Object
{
	ID3D11SamplerState *mSamplerState;
	ID3D11ShaderResourceView* mTexture;

	CommonMeshData meshData;
public:
	HeadModel();
	virtual void init() override;
	virtual void update(float Delta) override;
	virtual void render() override;
	virtual ~HeadModel();
};