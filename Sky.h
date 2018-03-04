#pragma once
#include "Object.h"
#include <vector>
struct SkyVertex
{
	AS3DVECTOR3 Pos;
	SkyVertex()
	{
		Pos.x = 0;
		Pos.y = 0;
		Pos.z = 0;
	}
	SkyVertex(float x, float y, float z)
	{
		Pos.x = x;
		Pos.y = y;
		Pos.z = z;
	}
};
class Sky : public Object
{

	ID3D11SamplerState *mSamplerState;
	ID3D11ShaderResourceView* mCubeTexture;

	ID3D11Buffer * mVertexBuffer;
	ID3D11Buffer * mIndexBuffer;
	ID3D11Buffer * mCBMatrixBuffer;

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;

	ID3D11InputLayout* mInputLayout;

	//Rasterize State
	ID3D11RasterizerState* mRasterizerState;

	//Blend State
	ID3D11BlendState* mBlendState;

	//depth stencil
	ID3D11DepthStencilState* mDepthStencilState;
	struct MeshData
	{
		std::vector<SkyVertex> vertices;
		std::vector<UINT> indices;
	};
	MeshData meshData;
	void GenerateMeshData(float radius, UINT sliceCount, UINT stackCount);
public:
	Sky(WorldTransform transform);
	virtual bool init() override;
	virtual void update(float Delta) override;
	virtual void render() override;
	virtual ~Sky();

	void recompileshader() {}
};

