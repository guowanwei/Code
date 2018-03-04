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
	, StartUpQuery(false)
{
	if (FbxFilePath == NULL || ShaderFilePath == NULL)
	{
		return;
	}
	mName = FbxFilePath;
	//create vertex buffer
	meshData.Clear();
	//load fbx
	FBXExporter fbxloader;
	fbxloader.Initialize();
	fbxloader.LoadModelInfo(FbxFilePath, meshData);
	
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
	
	D3D11_QUERY_DESC queryDesc;
	queryDesc.Query = D3D11_QUERY_OCCLUSION;
	queryDesc.MiscFlags = 0;
	HRESULT re = Device::Instance().GetDevice()->CreateQuery(&queryDesc, &mQuery);
	if (FAILED(re))
	{
		std::cout << "gww" << std::endl;
	}

	// 初始化混合状态  
	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.RenderTarget[0].BlendEnable = FALSE; 
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	result = Device::Instance().GetDevice()->CreateBlendState(
		&blendDesc,
		&mBlendState
	);
	//depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	Device::Instance().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	//boundbox
	InitAABB();
}

void Model::update(float Delta)
{
	//这里为什么需要转置矩阵呢？
	CBufferVertex1 cbufferVertex;
	cbufferVertex.WVProj = (GetWorldTransformMatrix() * Camera::Instance().GetViewProj()).GetTranspose();
	cbufferVertex.WorldMatrix = (GetWorldTransformMatrix()).GetTranspose();
	XMVECTOR tmp;
	cbufferVertex.WorldInverseTranspose = cbufferVertex.WorldMatrix.GetInverse().GetTranspose();
	Device::Instance().GetContext()->UpdateSubresource(mCBMatrixBufferVS, 0, NULL, &cbufferVertex, 0, 0);

	CBufferPixel1 cbufferPixel;
	cbufferPixel.LightColor = AS3DVECTOR4(8.0f, 8.0f, 8.0f, 0.0f);
	//cbufferPixel.LightColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	AS3DVECTOR3 tmp1 = Camera::Instance().GetEyePos();
	cbufferPixel.EyePosW.x = tmp1.x;
	cbufferPixel.EyePosW.y = tmp1.y;
	cbufferPixel.EyePosW.z = tmp1.z;
	cbufferPixel.LightDir = AS3DVECTOR4(-1.0f, -1.0f, -1.0f, 0.0f);
	//cbufferPixel.World = XMMatrixTranspose(GetWorldTransformMatrix());
	//cbufferPixel.WorldViewProj = XMMatrixTranspose(Camera::Instance().GetViewProj());

	Device::Instance().GetContext()->UpdateSubresource(mCBMatrixBufferPS, 0, NULL, &cbufferPixel, 0, 0);
}
void Model::InitAABB()
{
	AS3DVECTOR3 Corner1, Corner2;
	std::vector<BaseCommonVertex>::iterator iter = meshData.vertices.begin();
	Corner1 = Corner2 = iter->mPosition;
	for (; iter != meshData.vertices.end(); ++iter)
	{
		if (iter->mPosition.x > Corner1.x) Corner1.x = iter->mPosition.x;
		if (iter->mPosition.x < Corner2.x) Corner2.x = iter->mPosition.x;

		if (iter->mPosition.y > Corner1.y) Corner1.y = iter->mPosition.y;
		if (iter->mPosition.y < Corner2.y) Corner2.y = iter->mPosition.y;

		if (iter->mPosition.z > Corner1.z) Corner1.z = iter->mPosition.z;
		if (iter->mPosition.z < Corner2.z) Corner2.z = iter->mPosition.z;
	}
	//计算八个顶点
	AS3DVECTOR3 BoxVertex[8];
	BoxVertex[0]  = Corner1;
	BoxVertex[1] = Corner1; BoxVertex[1].x = Corner2.x;
	BoxVertex[2] = Corner1; BoxVertex[2].y = Corner2.y;
	BoxVertex[3] = Corner1; BoxVertex[3].z = Corner2.z;

	BoxVertex[4] = Corner2;
	BoxVertex[5] = Corner2; BoxVertex[5].x = Corner1.x;
	BoxVertex[6] = Corner2; BoxVertex[6].y = Corner1.y;
	BoxVertex[7] = Corner2; BoxVertex[7].z = Corner1.z;

	unsigned int BoxIndex[] =
	{
		1,7,4,4,6,1,
		6,4,5,5,3,6,
		3,5,2,2,0,3,
		1,6,3,3,0,1,
		7,1,0,0,2,7,
		4,7,2,2,5,4
	};
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(AS3DVECTOR3) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &BoxVertex[0];
	Device::Instance().GetDevice()->CreateBuffer(&vbd, &vinitData, &mBBVertexBuffer);

	//create index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(BoxIndex);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &BoxIndex[0];
	HRESULT a = Device::Instance().GetDevice()->CreateBuffer(&ibd, &iinitData, &mBBIndexBuffer);

	//compile and create shader
	HRESULT result;
	ID3D10Blob* errorMessage = NULL;
	ID3D10Blob* VertexShaderBuffer = NULL;
	ID3D10Blob* PixelShaderBuffer = NULL;

	const WCHAR* ShaderFilePath = L"Resource/Shader/BoundingBox.hlsl";
	result = D3DX11CompileFromFile(ShaderFilePath, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &VertexShaderBuffer, &errorMessage, NULL);
	Device::Instance().GetDevice()->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &mBBVertexShader);
	result = D3DX11CompileFromFile(ShaderFilePath, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &PixelShaderBuffer, &errorMessage, NULL);
	Device::Instance().GetDevice()->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &mBBPixelShader);

	//create input layout
	D3D11_INPUT_ELEMENT_DESC vertexInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	unsigned int numElements = sizeof(vertexInputLayout) / sizeof(vertexInputLayout[0]);
	result = Device::Instance().GetDevice()->CreateInputLayout(vertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &mBBInputLayout);
	VertexShaderBuffer->Release();
	VertexShaderBuffer = NULL;
	PixelShaderBuffer->Release();
	PixelShaderBuffer = NULL;

	//create resaterizer state
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode =  D3D11_FILL_SOLID;//
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.MultisampleEnable = false;
	result = Device::Instance().GetDevice()->CreateRasterizerState(&rasterDesc, &mBBRasterizerState);

	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0;//D3D11_COLOR_WRITE_ENABLE_ALL;

	result = Device::Instance().GetDevice()->CreateBlendState(
		&blendDesc,
		&mBBBlendState
	);

	//depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	Device::Instance().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &mBBDepthStencilState);
}
void Model::RenderAABB()
{
	Device::Instance().GetContext()->IASetInputLayout(mBBInputLayout);
	Device::Instance().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(AS3DVECTOR3);
	UINT offset = 0;
	Device::Instance().GetContext()->IASetVertexBuffers(0, 1, &mBBVertexBuffer, &stride, &offset);
	Device::Instance().GetContext()->IASetIndexBuffer(mBBIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Device::Instance().GetContext()->VSSetConstantBuffers(0, 1, &mCBMatrixBufferVS);

	//设置VertexShader和PixelShader
	Device::Instance().GetContext()->VSSetShader(mBBVertexShader, NULL, 0);
	Device::Instance().GetContext()->PSSetShader(mBBPixelShader, NULL, 0);
	Device::Instance().GetContext()->RSSetState(mBBRasterizerState);
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Device::Instance().GetContext()->OMSetBlendState(mBBBlendState, blendFactor, 0xffffffff);
	Device::Instance().GetContext()->OMSetDepthStencilState(mBBDepthStencilState, 0);
	//这里处理硬件遮蔽查询，查询时机需要间隔一帧！
	Device::Instance().GetContext()->Begin(mQuery);
	Device::Instance().GetContext()->DrawIndexed(36, 0, 0);
	Device::Instance().GetContext()->End(mQuery);
}
void Model::render()
{
	//设置顶点输入布局
	if (!bVisiable) return;
	//这里处理硬件遮蔽查询，查询时机需要间隔一帧！
	UINT64 Pixel = 0;
	if (StartUpQuery)
	{
		//这里是异步的，所以可以做些其他的事情
		while (Device::Instance().GetContext()->GetData(mQuery, &Pixel, sizeof(UINT64), D3D11_ASYNC_GETDATA_DONOTFLUSH) != S_OK) ;//
	}
	StartUpQuery = true;
	//首选渲染boundingbox
	RenderAABB();
	if (Pixel == 0) return;
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
	Device::Instance().GetContext()->OMSetDepthStencilState(mDepthStencilState, 0);

	for (std::map<UINT, MaterialDesc>::iterator iter = meshData.materials.begin(); iter != meshData.materials.end(); ++iter)
	{
		Device::Instance().GetContext()->PSSetShaderResources(0, 1, &mMaterials[iter->first].DiffuseMap);
		Device::Instance().GetContext()->PSSetShaderResources(1, 1, &mMaterials[iter->first].NormalMap);
		Device::Instance().GetContext()->PSSetShaderResources(2, 1, &mMaterials[iter->first].RougMetallicMap);
		Device::Instance().GetContext()->PSSetShaderResources(3, 1, &mCubeTexture);

		Device::Instance().GetContext()->DrawIndexed(iter->second.countIndex, iter->second.startIndex, 0);
	}
	static int x = 0;
	if (x == 1000)
	{
		std::cout << mName << std::endl;
	}
	x = (x + 1) % 1001;
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
	if (mQuery) mQuery->Release();
	mQuery = 0;
	if (mDepthStencilState) mDepthStencilState->Release();
	mDepthStencilState = NULL;


	if (mBBVertexBuffer) mBBVertexBuffer->Release();
	mBBVertexBuffer = NULL;
	if (mBBIndexBuffer)mBBIndexBuffer->Release();
	mBBIndexBuffer = NULL;
	if( mBBVertexShader) mBBVertexShader->Release();
	mBBVertexShader = NULL;
	if (mBBPixelShader) mBBPixelShader->Release();
	mBBPixelShader = NULL;
	if (mBBInputLayout) mBBInputLayout->Release();
	mBBInputLayout = NULL;
	if (mBBRasterizerState) mBBRasterizerState->Release();
	mBBRasterizerState = NULL;
	if (mBBBlendState) mBBBlendState->Release();
	mBBBlendState = NULL;
	if (mBBDepthStencilState) mBBDepthStencilState->Release();
	mBBDepthStencilState = NULL;
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