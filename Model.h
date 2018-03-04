#pragma once
#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>
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
		//这里读入内存的不是linear space,内存中存储的依然是sRGB
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
//cbuffer 对齐还是需要注意的
struct CBufferVertex1
{
	AS3DMATRIX4 WVProj;
	AS3DMATRIX4 WorldMatrix;
	AS3DMATRIX4 WorldInverseTranspose;
};

struct CBufferPixel1
{
	AS3DVECTOR4 LightDir;
	AS3DVECTOR4 LightColor;
	AS3DVECTOR4 EyePosW;
};
class Model : public Object
{
	std::string mName;
	bool StartUpQuery;
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
	//硬件遮蔽查询
	ID3D11Query* mQuery;

	ID3D11DepthStencilState* mDepthStencilState;
/**********************************************/
	//Bounding box
	ID3D11Buffer * mBBVertexBuffer;
	ID3D11Buffer * mBBIndexBuffer;
	//shaders
	ID3D11VertexShader* mBBVertexShader;
	ID3D11PixelShader* mBBPixelShader;

	ID3D11InputLayout* mBBInputLayout;
	ID3D11RasterizerState* mBBRasterizerState;
	ID3D11BlendState* mBBBlendState;
	ID3D11DepthStencilState* mBBDepthStencilState;

public:
	Model(const char* FbxFilePath, const WCHAR* ShaderFilePath, WorldTransform transform);
	virtual bool init() override;
	virtual void update(float Delta) override;
	virtual void render() override;
	virtual void recompileshader() override;
	virtual void InitAABB();
	virtual void RenderAABB();
	virtual ~Model();
};