#include "stdafx.h"
#include "CubeBox.h"
#include "Device.h"
#include "Camera.h"
#include "D3Dcompiler.h"
CubeBox::CubeBox()
	:mSamplerState(0),mTexture(0)
{
}
void CubeBox::init()
{
	//创建vertex buffer
	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),XMFLOAT4(0.0f,0.0f,0.0f,0.0f),XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f),XMFLOAT4(0.0f,0.0f,0.0f,0.0f),XMFLOAT2(0.0f,1.0f)},
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f),XMFLOAT4(0.0f,0.0f,0.0f,0.0f),XMFLOAT2(1.0f,1.0f)},
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f),XMFLOAT4(0.0f,0.0f,0.0f,0.0f),XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f),XMFLOAT4(0.0f,0.0f,0.0f,0.0f),XMFLOAT2(0.0f,0.0f)},
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f),XMFLOAT4(0.0f,0.0f,0.0f,0.0f),XMFLOAT2(0.0f,1.0f)},
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f),XMFLOAT4(0.0f,0.0f,0.0f,0.0f),XMFLOAT2(1.0f,1.0f)},
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f),XMFLOAT4(0.0f,0.0f,0.0f,0.0f),XMFLOAT2(1.0f,0.0f)}
	};
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	Device::Instance().GetDevice()->CreateBuffer(&vbd, &vinitData, &mVertexBuffer);

	//创建indexbuffer
	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		1, 5, 6,
		1, 6, 2,
		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HRESULT a = Device::Instance().GetDevice()->CreateBuffer(&ibd, &iinitData, &mIndexBuffer);

	//编译shader
	HRESULT result;
	ID3D10Blob* errorMessage = NULL;
	ID3D10Blob* VertexShaderBuffer = NULL;
	ID3D10Blob* PixelShaderBuffer = NULL;
	WCHAR* VSFileName = L"MyShader/CubeBox.hlsl";

	//第二,编译VertexShader代码,并创建VertexShader
	result = D3DX11CompileFromFile(VSFileName, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &VertexShaderBuffer, &errorMessage, NULL);
	Device::Instance().GetDevice()->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);

	WCHAR* PSFileName = L"MyShader/CubeBox.hlsl";
	//第三,编译PixelShader,并创建PixelShader
	result = D3DX11CompileFromFile(PSFileName, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &PixelShaderBuffer, &errorMessage, NULL);
	Device::Instance().GetDevice()->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);

	//输入布局！
	D3D11_INPUT_ELEMENT_DESC vertexInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,28,D3D11_INPUT_PER_VERTEX_DATA,0 },
		//{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,36,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	unsigned int numElements = sizeof(vertexInputLayout) / sizeof(vertexInputLayout[0]);         //布局数量

	Device::Instance().GetDevice()->CreateInputLayout(vertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &mInputLayout);
	VertexShaderBuffer->Release();
	VertexShaderBuffer = NULL;
	PixelShaderBuffer->Release();
	PixelShaderBuffer = NULL;

	//Create constant buffer
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixBufferDesc.ByteWidth = sizeof(XMMATRIX);   //结构体大小,必须为16字节倍数
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = 0;
	Device::Instance().GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, &mCBMatrixBuffer);

	//创建采样器状态
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
	Device::Instance().GetDevice()->CreateSamplerState(&samplerDesc, &mSamplerState);

	WCHAR* TextureFilename = L"Texture/Wood.dds";
	D3DX11CreateShaderResourceViewFromFileW(Device::Instance().GetDevice(), TextureFilename, NULL, NULL, &mTexture, NULL);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK; //背面剔除
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true; //深度裁剪开启
	rasterDesc.FillMode = D3D11_FILL_SOLID; //实体渲染
	rasterDesc.FrontCounterClockwise = false; //顺时针
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	Device::Instance().GetDevice()->CreateRasterizerState(&rasterDesc, &mRasterizerState);

}

void CubeBox::update(float Delta)
{
	/*
	XMVECTOR pos = XMVectorSet(10, 0, 0, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*3.1415f, 1.0f, 1.0f, 1000.0f);

	//XMMATRIX world = XMLoadFloat4x4(&mWorld);
	//XMMATRIX view = XMLoadFloat4x4(&mView);
	//XMMATRIX proj = XMLoadFloat4x4(&mProj);
	*/
	XMMATRIX worldViewProj = XMMatrixTranspose(Camera::Instance().GetViewProj());

	Device::Instance().GetContext()->UpdateSubresource(mCBMatrixBuffer, 0, NULL, &worldViewProj, 0, 0);
}
void CubeBox::render()
{
	//设置顶点输入布局
	Device::Instance().GetContext()->IASetInputLayout(mInputLayout);
	Device::Instance().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Device::Instance().GetContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	Device::Instance().GetContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Device::Instance().GetContext()->VSSetConstantBuffers(0, 1, &mCBMatrixBuffer);
	Device::Instance().GetContext()->PSSetSamplers(0, 1, &mSamplerState);
	Device::Instance().GetContext()->PSSetShaderResources(0, 1, &mTexture);
	//设置VertexShader和PixelShader
	Device::Instance().GetContext()->VSSetShader(mVertexShader, NULL, 0);
	Device::Instance().GetContext()->PSSetShader(mPixelShader, NULL, 0);
	Device::Instance().GetContext()->RSSetState(mRasterizerState);
	Device::Instance().GetContext()->DrawIndexed(36, 0, 0);
}
CubeBox::~CubeBox()
{
	if (mSamplerState) mSamplerState->Release();
	mSamplerState = 0;

	if (mTexture) mTexture->Release();
	mTexture = 0;
}