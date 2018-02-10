#include "stdafx.h"
#include "Sky.h"
#include "Device.h"
#include "Camera.h"
#include "D3Dcompiler.h"
Sky::Sky(WorldTransform transform)
	:Object(transform)
	,mSamplerState(0), mCubeTexture(0)
{
	GenerateMeshData(800.0f, 10, 10);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SkyVertex) * meshData.vertices.size();
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

	//����shader
	HRESULT result;
	ID3D10Blob* errorMessage = NULL;
	ID3D10Blob* VertexShaderBuffer = NULL;
	ID3D10Blob* PixelShaderBuffer = NULL;
	WCHAR* VSFileName = L"Resource/Shader/Sky.hlsl";

	//�ڶ�,����VertexShader����,������VertexShader
	result = D3DX11CompileFromFile(VSFileName, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &VertexShaderBuffer, &errorMessage, NULL);
	Device::Instance().GetDevice()->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);

	WCHAR* PSFileName = L"Resource/Shader/Sky.hlsl";
	//����,����PixelShader,������PixelShader
	result = D3DX11CompileFromFile(PSFileName, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &PixelShaderBuffer, &errorMessage, NULL);
	Device::Instance().GetDevice()->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);

	//���벼�֣�
	D3D11_INPUT_ELEMENT_DESC vertexInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = sizeof(vertexInputLayout) / sizeof(vertexInputLayout[0]);         //��������

	result = Device::Instance().GetDevice()->CreateInputLayout(vertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &mInputLayout);
	VertexShaderBuffer->Release();
	VertexShaderBuffer = NULL;
	PixelShaderBuffer->Release();
	PixelShaderBuffer = NULL;


	//Create constant buffer
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixBufferDesc.ByteWidth = sizeof(AS3DMATRIX4);   //�ṹ���С,����Ϊ16�ֽڱ���
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = 0;
	Device::Instance().GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, &mCBMatrixBuffer);


	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE; //�����޳�
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true; //��Ȳü�����
	rasterDesc.FillMode = D3D11_FILL_SOLID; //ʵ����Ⱦ
	rasterDesc.FrontCounterClockwise = false; //˳ʱ��
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	Device::Instance().GetDevice()->CreateRasterizerState(&rasterDesc, &mRasterizerState);

	WCHAR* cubemapFilename = L"Resource/Texture/desertcube1024.dds";
	D3DX11CreateShaderResourceViewFromFile(Device::Instance().GetDevice(), cubemapFilename, 0, 0, &mCubeTexture, 0);
}
bool Sky::init()
{
	return true;
}
void Sky::update(float Delta)
{
	//SetPosition(Camera::Instance().GetEyePos());
	AS3DMATRIX4 worldViewProj = (GetWorldTransformMatrix() * Camera::Instance().GetViewProj()).GetTranspose();
	Device::Instance().GetContext()->UpdateSubresource(mCBMatrixBuffer, 0, NULL, &worldViewProj, 0, 0);
}
void Sky::render()
{
	//���ö������벼��
	Device::Instance().GetContext()->IASetInputLayout(mInputLayout);
	Device::Instance().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(SkyVertex);
	UINT offset = 0;
	Device::Instance().GetContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	Device::Instance().GetContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Device::Instance().GetContext()->VSSetConstantBuffers(0, 1, &mCBMatrixBuffer);
	Device::Instance().GetContext()->PSSetSamplers(0, 1, &mSamplerState);
	Device::Instance().GetContext()->PSSetShaderResources(0, 1, &mCubeTexture);
	//����VertexShader��PixelShader
	Device::Instance().GetContext()->VSSetShader(mVertexShader, NULL, 0);
	Device::Instance().GetContext()->PSSetShader(mPixelShader, NULL, 0);
	Device::Instance().GetContext()->RSSetState(mRasterizerState);
	Device::Instance().GetContext()->DrawIndexed(meshData.indices.size(), 0, 0);
}
Sky::~Sky()
{
	if (mSamplerState) mSamplerState->Release();
	mSamplerState = 0;

	if (mCubeTexture) mCubeTexture->Release();
	mCubeTexture = 0;



	if (mVertexBuffer) mVertexBuffer->Release();
	mVertexBuffer = 0;
	if (mIndexBuffer) mIndexBuffer->Release();
	mIndexBuffer = 0;
	if (mCBMatrixBuffer) mCBMatrixBuffer->Release();
	mCBMatrixBuffer = 0;
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
void Sky::GenerateMeshData(float radius, UINT sliceCount, UINT stackCount)
{
	meshData.vertices.clear();
	meshData.indices.clear();

	SkyVertex topVertex(0.0f, +radius, 0.0f);
	SkyVertex bottomVertex(0.0f, -radius, 0.0f);

	meshData.vertices.push_back(topVertex);

	float phiStep = PI / stackCount;
	float thetaStep = 2.0f*PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			SkyVertex v;
			v.Pos.x = radius*sinf(phi)*cosf(theta);
			v.Pos.y = radius*cosf(phi);
			v.Pos.z = radius*sinf(phi)*sinf(theta);

			meshData.vertices.push_back(v);
		}
	}

	meshData.vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		meshData.indices.push_back(0);
		meshData.indices.push_back(i + 1);
		meshData.indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			meshData.indices.push_back(baseIndex + i*ringVertexCount + j);
			meshData.indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			meshData.indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)meshData.vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.indices.push_back(southPoleIndex);
		meshData.indices.push_back(baseIndex + i);
		meshData.indices.push_back(baseIndex + i + 1);
	}
}