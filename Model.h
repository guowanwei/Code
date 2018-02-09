#pragma once
#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>
#include <xnamath.h>
#include "BaseUtilities.h"
#include "Device.h"
#include "Object.h"
struct MaterialTexture
{
	MaterialTexture()
		:DiffuseMap(0), NormalMap(0), RougMetallicMap(0)
	{}
	~MaterialTexture()
	{
		if (DiffuseMap) DiffuseMap->Release();
		DiffuseMap = 0;
		if (NormalMap) NormalMap->Release();
		NormalMap = 0;
		if (RougMetallicMap) RougMetallicMap->Release();
		RougMetallicMap = 0;
	}
	bool init(const WCHAR* diffMap, const WCHAR* normalMap, const WCHAR* roughMetallic)
	{
		//D3DX11_IMAGE_LOAD_INFO info;
		//info.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;// DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
		//��������ڴ�Ĳ���linear space,�ڴ��д洢����Ȼ��sRGB
		D3DX11CreateShaderResourceViewFromFileW(Device::Instance().GetDevice(), diffMap, NULL, NULL, &DiffuseMap, NULL);
		D3DX11CreateShaderResourceViewFromFileW(Device::Instance().GetDevice(), normalMap, NULL, NULL, &NormalMap, NULL);
		D3DX11CreateShaderResourceViewFromFileW(Device::Instance().GetDevice(), roughMetallic, NULL, NULL, &RougMetallicMap, NULL);
		return true;
	}
private:
	MaterialTexture(const MaterialTexture& r) {}
public:
	ID3D11ShaderResourceView* DiffuseMap;
	ID3D11ShaderResourceView* NormalMap;
	ID3D11ShaderResourceView* RougMetallicMap;
};
//cbuffer ���뻹����Ҫע���
struct CBufferVertex1
{
	XMMATRIX WVProj;
	XMMATRIX WorldMatrix;
	XMMATRIX WorldInverseTranspose;
};

struct CBufferPixel1
{
	XMFLOAT4 LightDir;
	XMFLOAT4 LightColor;
	XMFLOAT4 EyePosW;
};
class Model : public Object
{
	//geometry info cache
	CommonMeshData meshData;

protected:
	std::map<UINT, MaterialTexture>  mMaterials;
	//one sampler state
	ID3D11SamplerState*mSamplerState;
	ID3D11ShaderResourceView* mCubeTexture;

	//buffers
	ID3D11Buffer * mVertexBuffer;
	ID3D11Buffer * mIndexBuffer;
	ID3D11Buffer * mCBMatrixBufferVS;
	ID3D11Buffer * mCBMatrixBufferPS;

	//shaders
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11GeometryShader* mGeometryShader;

	//inputlayout
	ID3D11InputLayout* mInputLayout;

	//Rasterize State
	ID3D11RasterizerState* mRasterizerState;

	//Blend State
	ID3D11BlendState* mBlendState;

public:
	Model(const char* FbxFilePath, const WCHAR* ShaderFilePath, WorldTransform transform);
	virtual bool init() override;
	virtual void update(float Delta) override;
	virtual void render() override;
	virtual void recompileshader() override;
	virtual ~Model();
};