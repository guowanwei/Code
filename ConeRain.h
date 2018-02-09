#pragma once
#include "Object.h"
#include <vector>
#include <iostream>
struct CBufferVertexVS_ConeRain
{
	XMMATRIX WVProj;
	XMMATRIX WorldInverseTranspose;
	XMMATRIX WorldMatrix;
	XMFLOAT3 Time;
};

struct CBufferVertexPS_ConeRain
{
	XMFLOAT4 RainIntensity;
};
struct ConeRainVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 UVW;
	ConeRainVertex()
	{
		Pos.x = 0;
		Pos.y = 0;
		Pos.z = 0;

		UVW.x = 0;
		UVW.y = 0;
		UVW.z = 0;
	}
	ConeRainVertex(float x, float y, float z, float u,float v)
	{
		Pos.x = x;
		Pos.y = y;
		Pos.z = z;

		UVW.x = u;
		UVW.y = v;
		UVW.z = 0;
	}
};
class ConeRain : public Object
{
private:
	float Intensify;
	ID3D11SamplerState *mSamplerState;
	ID3D11ShaderResourceView* mTexture;

	ID3D11Buffer * mVertexBuffer;
	ID3D11Buffer * mIndexBuffer;
	ID3D11Buffer * mCBMatrixBufferVS;

	ID3D11Buffer * mCBMatrixBufferPS;

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;

	ID3D11InputLayout* mInputLayout;

	//Rasterize State
	ID3D11RasterizerState* mRasterizerState;

	//Blend State
	ID3D11BlendState* mBlendState;

	struct MeshData
	{
		std::vector<ConeRainVertex> vertices;
		std::vector<UINT> indices;
	};
	MeshData meshData;
	void GenRainCameraCone();
	void GenRainCameraCone2();
	void GenRainCameraCone3();
	void GenRainCameraCone4();
public:
	ConeRain(WorldTransform transform, float VOffSet);
	virtual bool init() override;
	virtual void update(float Delta) override;
	virtual void render() override;
	virtual ~ConeRain();
	void recompileshader() {}
	void AddIntensity(bool add) {
		if (add)
		{
			if(Intensify < 16)
				Intensify++;
		}
		else
		{
			if (Intensify > 1)
				Intensify--;
		}
		std::cout << Intensify << std::endl;
	}
};