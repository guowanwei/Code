#include "stdafx.h"
#include "ConeRain.h"
#include "Device.h"
#include "Camera.h"
#include "D3Dcompiler.h"
#include <fstream>
#include "FBX/FBXExporter.h"
ConeRain::ConeRain(WorldTransform transform,float VOffSet)
	:Object(transform)
	, mSamplerState(0), mTexture(0)
	, Intensify(8)
{
	//GenerateMeshData(10.0f, 100, 100);
	GenRainCameraCone3();

	//GenRainCameraCone4();


	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(ConeRainVertex) * meshData.vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &meshData.vertices[0];
	Device::Instance().GetDevice()->CreateBuffer(&vbd, &vinitData, &mVertexBuffer);

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
	a = Device::Instance().GetDevice()->CreateSamplerState(&samplerDesc, &mSamplerState);

	//编译shader
	HRESULT result;
	ID3D10Blob* errorMessage = NULL;
	ID3D10Blob* VertexShaderBuffer = NULL;
	ID3D10Blob* PixelShaderBuffer = NULL;
	WCHAR* HLSLFileName = L"Resource/Shader/ConeRain.hlsl";
	//WCHAR* HLSLFileName = L"Resource/Shader/Sky.hlsl";

	//第二,编译VertexShader代码,并创建VertexShader
	result = D3DX11CompileFromFile(HLSLFileName, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &VertexShaderBuffer, &errorMessage, NULL);
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
	//第三,编译PixelShader,并创建PixelShader
	result = D3DX11CompileFromFile(HLSLFileName, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &PixelShaderBuffer, &errorMessage, NULL);
	Device::Instance().GetDevice()->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);

	//输入布局！
	D3D11_INPUT_ELEMENT_DESC vertexInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = sizeof(vertexInputLayout) / sizeof(vertexInputLayout[0]);         //布局数量

	result = Device::Instance().GetDevice()->CreateInputLayout(vertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &mInputLayout);
	VertexShaderBuffer->Release();
	VertexShaderBuffer = NULL;
	PixelShaderBuffer->Release();
	PixelShaderBuffer = NULL;


	//Create constant buffer
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixBufferDesc.ByteWidth = sizeof(CBufferVertexVS_ConeRain);   //结构体大小,必须为16字节倍数
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = 0;
	Device::Instance().GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, &mCBMatrixBufferVS);

	//Create constant buffer
	D3D11_BUFFER_DESC PSBufferDesc;
	ZeroMemory(&PSBufferDesc, sizeof(PSBufferDesc));
	PSBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	PSBufferDesc.ByteWidth = sizeof(CBufferVertexPS_ConeRain);   //结构体大小,必须为16字节倍数
	PSBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PSBufferDesc.CPUAccessFlags = 0;
	Device::Instance().GetDevice()->CreateBuffer(&PSBufferDesc, NULL, &mCBMatrixBufferPS);



	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE; //背面剔除
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true; //深度裁剪开启
	rasterDesc.FillMode = D3D11_FILL_SOLID; //实体渲染
	rasterDesc.FrontCounterClockwise = false; //顺时针
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	Device::Instance().GetDevice()->CreateRasterizerState(&rasterDesc, &mRasterizerState);

	//WCHAR* cubemapFilename = L"Resource/Texture/dds//Converted/hdr.dds";
	WCHAR* TextureFilename = L"Resource/Texture/ConeRain.dds";
	result = D3DX11CreateShaderResourceViewFromFile(Device::Instance().GetDevice(), TextureFilename, 0, 0, &mTexture, 0);
	if (FAILED(result))
	{
		return;
	}

	// 初始化混合状态  
	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;// D3D11_BLEND_SRC_COLOR; // Color_Fsrc  
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;// D3D11_BLEND_DEST_COLOR; // Color_Fdst  
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // Color_Operation  
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fsrc  
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // Alpha_Fdst  
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // Alpha_Operation  
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	result = Device::Instance().GetDevice()->CreateBlendState(
		&blendDesc,
		&mBlendState
	);
}
bool ConeRain::init()
{
	return true;
}
void ConeRain::update(float Delta)
{
	static float time = 0;
	time += Delta;
	SetPosition(Camera::Instance().GetEyePos());
	CBufferVertexVS_ConeRain cbufferVertex;
	cbufferVertex.WVProj = XMMatrixTranspose(GetWorldTransformMatrix() * Camera::Instance().GetViewProj());
	XMVECTOR tmp;
	cbufferVertex.WorldInverseTranspose = XMMatrixTranspose(XMMatrixInverse(&tmp, XMMatrixTranspose(GetWorldTransformMatrix())));
	cbufferVertex.WorldMatrix = XMMatrixTranspose(GetWorldTransformMatrix());
	cbufferVertex.Time.x = time / 1000.0f;
	Device::Instance().GetContext()->UpdateSubresource(mCBMatrixBufferVS, 0, NULL, &cbufferVertex, 0, 0);

	CBufferVertexPS_ConeRain psBuffer;
	psBuffer.RainIntensity.x = Intensify;
	Device::Instance().GetContext()->UpdateSubresource(mCBMatrixBufferPS, 0, NULL, &psBuffer, 0, 0);
}
void ConeRain::render()
{
	//设置顶点输入布局
	Device::Instance().GetContext()->IASetInputLayout(mInputLayout);
	Device::Instance().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(ConeRainVertex);
	UINT offset = 0;
	Device::Instance().GetContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	Device::Instance().GetContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Device::Instance().GetContext()->VSSetConstantBuffers(0, 1, &mCBMatrixBufferVS);
	Device::Instance().GetContext()->PSSetConstantBuffers(0, 1, &mCBMatrixBufferPS);
	Device::Instance().GetContext()->PSSetSamplers(0, 1, &mSamplerState);
	Device::Instance().GetContext()->PSSetShaderResources(0, 1, &mTexture);
	//设置VertexShader和PixelShader
	Device::Instance().GetContext()->VSSetShader(mVertexShader, NULL, 0);
	Device::Instance().GetContext()->PSSetShader(mPixelShader, NULL, 0);
	Device::Instance().GetContext()->RSSetState(mRasterizerState);

	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Device::Instance().GetContext()->OMSetBlendState(mBlendState,blendFactor, 0xffffffff);
	Device::Instance().GetContext()->DrawIndexed(meshData.indices.size(), 0, 0);
}
ConeRain::~ConeRain()
{
	if (mSamplerState) mSamplerState->Release();
	mSamplerState = 0;

	if (mTexture) mTexture->Release();
	mTexture = 0;



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
	if (mRasterizerState) mRasterizerState->Release();
	mRasterizerState = 0;
	if (mBlendState) mBlendState->Release();
	mBlendState = 0;
}

void ConeRain::GenRainCameraCone()
{
	float PI_Eight = PI / 8;
	meshData.vertices.clear();
	meshData.indices.clear();
	//x -0.75 ~ 0.75  y -0.75 ~ 0.75  z -1 ~ 1
	float z_steps[] = { 1.0,0.75,0.50,0.25,-0.25,-0.5,-0.75,-1.0 };
	for (int i = 0; i < sizeof(z_steps) / sizeof(float); ++i)
	{
		float U_Start = (abs(z_steps[i]) - 0.5) / 3.0;
		float U_End =  1 - U_Start;
		for (int j = 0; j <= 16; ++j)
		{
			ConeRainVertex SV;
			XMFLOAT3 Pos;
			Pos.z = z_steps[i];
			float radius = 1.0 - abs(Pos.z);
			Pos.x = sin(j * PI_Eight) * radius;
			Pos.y = cos(j * PI_Eight) * radius;
			XMFLOAT3 UV;
			UV.x = U_Start + j * (U_End - U_Start)/16.0;
			UV.y = 0.5 - Pos.z * 0.5;
			SV.Pos = Pos;
			SV.UVW = UV;
			meshData.vertices.push_back(SV);
		}
	}
	for (int i = 0; i < sizeof(z_steps) / sizeof(float) - 1; ++i)
	{
		int firstPosIndex = 17 * i;
		for (int j = 0; j < 16; ++j)
		{

			meshData.indices.push_back(firstPosIndex + j);
			meshData.indices.push_back(firstPosIndex + j + 1);
			meshData.indices.push_back(firstPosIndex + j + 17);

			meshData.indices.push_back(firstPosIndex + j + 1);
			meshData.indices.push_back(firstPosIndex + j + 1 + 17);
			meshData.indices.push_back(firstPosIndex + j + 17);
		}
	}
}

void ConeRain::GenRainCameraCone2()
{
	float PI_Eight = PI / 8;
	meshData.vertices.clear();
	meshData.indices.clear();
	//x -0.75 ~ 0.75  y -0.75 ~ 0.75  z -1 ~ 1
	float z_steps[] = { 1.0,0.75,0.50,0.25,-0.25,-0.5,-0.75,-1.0 };
	for (int i = 0; i < sizeof(z_steps) / sizeof(float); ++i)
	{
		for (int j = 0; j <= 16; ++j)
		{
			// first point
			{
				ConeRainVertex SV;
				XMFLOAT3 Pos;
				Pos.z = z_steps[i];
				float radius = 1.0 - abs(Pos.z);
				Pos.x = sin(j * PI_Eight) * radius;
				Pos.y = cos(j * PI_Eight) * radius;
				XMFLOAT3 UV;
				UV.x = (j * 2 + 1) / 32.0 - (1 - Pos.z) / 24.0;
				UV.y = 0.5 - Pos.z * 0.5;
				SV.Pos = Pos;
				SV.UVW = UV;
				meshData.vertices.push_back(SV);
			}
			// second point
			if (j != 0 && j != 16)
			{
				ConeRainVertex SV;
				XMFLOAT3 Pos;
				Pos.z = z_steps[i];
				float radius = 1.0 - abs(Pos.z);
				Pos.x = sin(j * PI_Eight) * radius;
				Pos.y = cos(j * PI_Eight) * radius;
				XMFLOAT3 UV;
				UV.x = (j * 2 + 1) / 32.0 + (1 - Pos.z) / 24.0;
				UV.y = 0.5 - Pos.z * 0.5;
				SV.Pos = Pos;
				SV.UVW = UV;
				meshData.vertices.push_back(SV);
			}
		}
	}
	for (int i = 0; i < sizeof(z_steps) / sizeof(float) - 1; ++i)
	{
		int firstPosIndex = 32 * i;
		for (int j = 0; j < 16; ++j)
		{

			meshData.indices.push_back(firstPosIndex + j * 2);
			meshData.indices.push_back(firstPosIndex + j * 2 + 1);
			meshData.indices.push_back(firstPosIndex + j * 2 + 32);

			meshData.indices.push_back(firstPosIndex + j * 2 + 1);
			meshData.indices.push_back(firstPosIndex + j * 2 + 1 + 32);
			meshData.indices.push_back(firstPosIndex + j * 2 + 32);
		}
	}
}

void ConeRain::GenRainCameraCone3()
{
	float PI_Eight = PI / 8;
	meshData.vertices.clear();
	meshData.indices.clear();
	//x -0.75 ~ 0.75  y -0.75 ~ 0.75  z -1 ~ 1
	float z_steps[] = { 1.0,0.75,0.50,0.25,-0.25,-0.5,-0.75,-1.0 };
	float Transparent[] = { 0.1,0.2,0.50,0.6,0.6,0.5,0.2,0.1 };
	for (int i = 0; i < sizeof(z_steps) / sizeof(float); ++i)
	{
		for (int j = 0; j <= 16; ++j)
		{
			ConeRainVertex SV;
			XMFLOAT3 Pos;
			Pos.z = z_steps[i];
			float radius = 1.0 - abs(Pos.z);
			Pos.x = sin(j * PI_Eight) * radius;
			Pos.y = cos(j * PI_Eight) * radius;
			XMFLOAT3 UVW;
			UVW.x = j / 16.0;
			if (j == 0) UVW.x += 1 / 32.0;
			UVW.y = 0.5 - Pos.z * 0.5;
			UVW.z = Transparent[i];
			SV.Pos = Pos;
			SV.UVW = UVW;
			meshData.vertices.push_back(SV);
		}
	}
	for (int i = 0; i < sizeof(z_steps) / sizeof(float) - 1; ++i)
	{
		int firstPosIndex = 17 * i;
		for (int j = 0; j < 16; ++j)
		{
			meshData.indices.push_back(firstPosIndex + j);
			meshData.indices.push_back(firstPosIndex + j + 1);
			meshData.indices.push_back(firstPosIndex + j + 17);

			meshData.indices.push_back(firstPosIndex + j + 1);
			meshData.indices.push_back(firstPosIndex + j + 1 + 17);
			meshData.indices.push_back(firstPosIndex + j + 17);
		}
	}
}

void ConeRain::GenRainCameraCone4()
{
	float PI_Eight = PI / 8;
	meshData.vertices.clear();
	meshData.indices.clear();
	//x -0.75 ~ 0.75  y -0.75 ~ 0.75  z -1 ~ 1
	float z_steps[] = { 1.0,0.75,0.50,0.25,-0.25,-0.5,-0.75,-1.0 };
	float Transparent[] = { 0.1,0.2,0.50,0.6,0.6,0.5,0.2,0.1 };
	for (int i = 0; i < sizeof(z_steps) / sizeof(float); ++i)
	{
		for (int j = 0; j <= 16; ++j)
		{
			ConeRainVertex SV;
			XMFLOAT3 Pos;
			Pos.z = z_steps[i];
			float radius = 1.0 - abs(Pos.z);
			Pos.x = sin(j * PI_Eight) * radius;
			Pos.y = cos(j * PI_Eight) * radius;
			XMFLOAT3 UVW;
			UVW.x = j / 16.0;
			if (j == 0) UVW.x += 1 / 32.0;
			UVW.y = 0.5 - Pos.z * 0.5;
			UVW.z = Transparent[i];
			SV.Pos = Pos;
			SV.UVW = UVW;
			if (i == 0 || i == 7)
			{
				SV.UVW.x = 0.5;
				SV.UVW.y =(1 - z_steps[i]) /2.0;

			}
			meshData.vertices.push_back(SV);
		}
	}
	for (int i = 0; i < sizeof(z_steps) / sizeof(float) - 1; ++i)
	{
		int firstPosIndex = 17 * i;
		for (int j = 0; j < 16; ++j)
		{
			meshData.indices.push_back(firstPosIndex + j);
			meshData.indices.push_back(firstPosIndex + j + 1);
			meshData.indices.push_back(firstPosIndex + j + 17);

			meshData.indices.push_back(firstPosIndex + j + 1);
			meshData.indices.push_back(firstPosIndex + j + 1 + 17);
			meshData.indices.push_back(firstPosIndex + j + 17);
		}
	}
}