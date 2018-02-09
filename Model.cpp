#include "stdafx.h"
#include"Model.h"
#include "D3Dcompiler.h"
#include "Device.h"
#include "Camera.h"
#include "FBX/FBXExporter.h"
#include <fstream>
Model::Model(const char* FbxFilePath, const WCHAR* ShaderFilePath, WorldTransform transform)
	:Object(transform),
	mVertexBuffer(0), mIndexBuffer(0)
	, mCBMatrixBufferVS(0), mVertexShader(0)
	, mCBMatrixBufferPS(0)
	, mPixelShader(0), mInputLayout(0)
	, mGeometryShader(0), mRasterizerState(0)
	, mBlendState(0), mSamplerState(0)
{
	if (FbxFilePath == NULL || ShaderFilePath == NULL)
	{
		return;
	}
	//create vertex buffer
	meshData.Clear();
	//load fbx
	FBXExporter fbxloader;
	fbxloader.Initialize();
	fbxloader.LoadModelInfo(FbxFilePath, meshData);
	//
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(BaseCommonVertex) * meshData.vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &meshData.vertices[0];
	Device::Instance().GetDevice()->CreateBuffer(&vbd, &vinitData, &mVertexBuffer);

	//create index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * meshData.indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &meshData.indices[0];
	HRESULT a = Device::Instance().GetDevice()->CreateBuffer(&ibd, &iinitData, &mIndexBuffer);

	//create const buffer vs
	D3D11_BUFFER_DESC matrixBufferDescVS;
	ZeroMemory(&matrixBufferDescVS, sizeof(matrixBufferDescVS));
	matrixBufferDescVS.Usage = D3D11_USAGE_DEFAULT; //这个应该改成dynamic？
	matrixBufferDescVS.ByteWidth = sizeof(CBufferVertex1);
	matrixBufferDescVS.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDescVS.CPUAccessFlags = 0;
	Device::Instance().GetDevice()->CreateBuffer(&matrixBufferDescVS, NULL, &mCBMatrixBufferVS);

	//create const buffer
	D3D11_BUFFER_DESC matrixBufferDescPS;
	ZeroMemory(&matrixBufferDescPS, sizeof(matrixBufferDescPS));
	matrixBufferDescPS.Usage = D3D11_USAGE_DEFAULT; //这个应该改成dynamic？
	matrixBufferDescPS.ByteWidth = sizeof(CBufferPixel1);
	matrixBufferDescPS.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDescPS.CPUAccessFlags = 0;
	Device::Instance().GetDevice()->CreateBuffer(&matrixBufferDescPS, NULL, &mCBMatrixBufferPS);

	//compile and create shader
	HRESULT result;
	ID3D10Blob* errorMessage = NULL;
	ID3D10Blob* VertexShaderBuffer = NULL;
	ID3D10Blob* PixelShaderBuffer = NULL;
	D3D10_SHADER_MACRO* Macros;
	if (bUseIBL)
	{
		D3D10_SHADER_MACRO Shader_Macros[] = { "_IBL", "0",NULL,NULL };
		Macros = Shader_Macros;
	}
	else
	{
		D3D10_SHADER_MACRO Shader_Macros[] = { NULL,NULL };
		Macros = Shader_Macros;
	}

	//D3D10_SHADER_MACRO Shader_Macros[] = {NULL,NULL };
	result = D3DX11CompileFromFile(ShaderFilePath, Macros, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &VertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// 获取指向错误信息文本的指针
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());

		// 获取错误信息文本的长度
		int bufferSize = errorMessage->GetBufferSize();

		std::ofstream fout;

		// 创建一个txt,用于写入错误信息
		fout.open("shader-error.txt");

		//想txt文件写入错误信息
		for (int i = 0; i < bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		// 关闭文件
		fout.close();
	}
	Device::Instance().GetDevice()->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
	result = D3DX11CompileFromFile(ShaderFilePath, Macros, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &PixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// 获取指向错误信息文本的指针
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());

		// 获取错误信息文本的长度
		int bufferSize = errorMessage->GetBufferSize();

		std::ofstream fout;

		// 创建一个txt,用于写入错误信息
		fout.open("shader-error.txt");

		//想txt文件写入错误信息
		for (int i = 0; i < bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		// 关闭文件
		fout.close();
	}

	Device::Instance().GetDevice()->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);

	//create input layout
	D3D11_INPUT_ELEMENT_DESC vertexInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,36,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,48,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	unsigned int numElements = sizeof(vertexInputLayout) / sizeof(vertexInputLayout[0]);
	result = Device::Instance().GetDevice()->CreateInputLayout(vertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &mInputLayout);
	VertexShaderBuffer->Release();
	VertexShaderBuffer = NULL;
	PixelShaderBuffer->Release();
	PixelShaderBuffer = NULL;

	//create sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = Device::Instance().GetDevice()->CreateSamplerState(&samplerDesc, &mSamplerState);

	//create materials
	for (std::map<UINT, MaterialDesc>::iterator iter = meshData.materials.begin(); iter != meshData.materials.end(); ++iter)
	{
		mMaterials[iter->first].init(string_to_wstring(iter->second.mDiffuseMapName).c_str(), string_to_wstring(iter->second.mNormalMapName).c_str(), string_to_wstring(iter->second.RoughnessMetallicMapName).c_str());
	}

	//create resaterizer state
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.MultisampleEnable = false;
	result = Device::Instance().GetDevice()->CreateRasterizerState(&rasterDesc, &mRasterizerState);


	WCHAR* cubemapFilename = L"Resource/Texture/snowcube1024.dds";
	D3DX11CreateShaderResourceViewFromFile(Device::Instance().GetDevice(), cubemapFilename, 0, 0, &mCubeTexture, 0);
}

void Model::update(float Delta)
{
	//这里为什么需要转置矩阵呢？
	CBufferVertex1 cbufferVertex;
	cbufferVertex.WVProj = XMMatrixTranspose(GetWorldTransformMatrix() * Camera::Instance().GetViewProj());
	cbufferVertex.WorldMatrix = XMMatrixTranspose(GetWorldTransformMatrix());
	XMVECTOR tmp;
	cbufferVertex.WorldInverseTranspose = XMMatrixTranspose(XMMatrixInverse(&tmp, cbufferVertex.WorldMatrix));
	Device::Instance().GetContext()->UpdateSubresource(mCBMatrixBufferVS, 0, NULL, &cbufferVertex, 0, 0);

	CBufferPixel1 cbufferPixel;
	cbufferPixel.LightColor = XMFLOAT4(8.0f, 8.0f, 8.0f, 0.0f);
	//cbufferPixel.LightColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	XMFLOAT3 tmp1 = Camera::Instance().GetEyePos();
	cbufferPixel.EyePosW.x = tmp1.x;
	cbufferPixel.EyePosW.y = tmp1.y;
	cbufferPixel.EyePosW.z = tmp1.z;
	cbufferPixel.LightDir = XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);
	//cbufferPixel.World = XMMatrixTranspose(GetWorldTransformMatrix());
	//cbufferPixel.WorldViewProj = XMMatrixTranspose(Camera::Instance().GetViewProj());

	Device::Instance().GetContext()->UpdateSubresource(mCBMatrixBufferPS, 0, NULL, &cbufferPixel, 0, 0);
}
void Model::render()
{
	//设置顶点输入布局
	if (!bVisiable) return;
	Device::Instance().GetContext()->IASetInputLayout(mInputLayout);
	Device::Instance().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(BaseCommonVertex);
	UINT offset = 0;
	Device::Instance().GetContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	Device::Instance().GetContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Device::Instance().GetContext()->VSSetConstantBuffers(0, 1, &mCBMatrixBufferVS);
	Device::Instance().GetContext()->PSSetConstantBuffers(0, 1, &mCBMatrixBufferPS);
	Device::Instance().GetContext()->PSSetSamplers(0, 1, &mSamplerState);

	//设置VertexShader和PixelShader
	Device::Instance().GetContext()->VSSetShader(mVertexShader, NULL, 0);
	Device::Instance().GetContext()->PSSetShader(mPixelShader, NULL, 0);
	Device::Instance().GetContext()->RSSetState(mRasterizerState);
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Device::Instance().GetContext()->OMSetBlendState(mBlendState, blendFactor, 0xffffffff);
	for (std::map<UINT, MaterialDesc>::iterator iter = meshData.materials.begin(); iter != meshData.materials.end(); ++iter)
	{
		Device::Instance().GetContext()->PSSetShaderResources(0, 1, &mMaterials[iter->first].DiffuseMap);
		Device::Instance().GetContext()->PSSetShaderResources(1, 1, &mMaterials[iter->first].NormalMap);
		Device::Instance().GetContext()->PSSetShaderResources(2, 1, &mMaterials[iter->first].RougMetallicMap);
		Device::Instance().GetContext()->PSSetShaderResources(3, 1, &mCubeTexture);

		Device::Instance().GetContext()->DrawIndexed(iter->second.countIndex, iter->second.startIndex, 0);
	}
}
bool Model::init()
{
	return true;
}
Model::~Model()
{
	if (mVertexBuffer) mVertexBuffer->Release();
	mVertexBuffer = 0;

	if (mIndexBuffer) mIndexBuffer->Release();
	mIndexBuffer = 0;
	if (mCBMatrixBufferVS) mCBMatrixBufferVS->Release();
	mCBMatrixBufferVS = 0;

	if (mCBMatrixBufferPS) mCBMatrixBufferPS->Release();
	mCBMatrixBufferPS = 0;

	if (mVertexShader) mVertexShader->Release();
	mVertexShader = 0;

	if (mPixelShader) mPixelShader->Release();
	mPixelShader = 0;
	if (mInputLayout) mInputLayout->Release();
	mInputLayout = 0;
	if (mGeometryShader) mGeometryShader->Release();
	mGeometryShader = 0;

	if (mRasterizerState) mRasterizerState->Release();
	mRasterizerState = 0;
	if (mBlendState) mBlendState->Release();
	mBlendState = 0;
	if (mSamplerState) mSamplerState->Release();
	mSamplerState = 0;
	if (mCubeTexture) mCubeTexture->Release();
	mCubeTexture = 0;
}
void Model::recompileshader()
{
	if (mVertexShader) mVertexShader->Release();
	mVertexShader = 0;

	if (mPixelShader) mPixelShader->Release();
	mPixelShader = 0;

	//compile and create shader
	HRESULT result;
	ID3D10Blob* errorMessage = NULL;
	ID3D10Blob* VertexShaderBuffer = NULL;
	ID3D10Blob* PixelShaderBuffer = NULL;
	D3D10_SHADER_MACRO* Macros;
	if (!bUseIBL)
	{
		D3D10_SHADER_MACRO Shader_Macros[] = { "_IBL", "0",NULL,NULL };
		Macros = Shader_Macros;
		bUseIBL = true;
	}
	else
	{
		D3D10_SHADER_MACRO Shader_Macros[] = { NULL,NULL };
		Macros = Shader_Macros;
		bUseIBL = false;
	}
	//D3D10_SHADER_MACRO Shader_Macros[] = {NULL,NULL };
	result = D3DX11CompileFromFile(L"Resource/Shader/Common.hlsl", Macros, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &VertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// 获取指向错误信息文本的指针
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());

		// 获取错误信息文本的长度
		int bufferSize = errorMessage->GetBufferSize();

		std::ofstream fout;

		// 创建一个txt,用于写入错误信息
		fout.open("shader-error.txt");

		//想txt文件写入错误信息
		for (int i = 0; i < bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		// 关闭文件
		fout.close();
	}
	Device::Instance().GetDevice()->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
	result = D3DX11CompileFromFile(L"Resource/Shader/Common.hlsl", Macros, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &PixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// 获取指向错误信息文本的指针
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());

		// 获取错误信息文本的长度
		int bufferSize = errorMessage->GetBufferSize();

		std::ofstream fout;

		// 创建一个txt,用于写入错误信息
		fout.open("shader-error.txt");

		//想txt文件写入错误信息
		for (int i = 0; i < bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		// 关闭文件
		fout.close();
	}
	Device::Instance().GetDevice()->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);
}