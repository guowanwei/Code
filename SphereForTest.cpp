#include "stdafx.h"
#include "SphereForTest.h"
#include "Device.h"
#include "Camera.h"
#include "D3Dcompiler.h"
#include <fstream>
SphereForTest::SphereForTest(WorldTransform transform, float roughness, AS3DVECTOR3 specularColor)
	:Object(transform)
	, mSamplerState(0), mCubeTexture(0)
{
	this->roughness = roughness;
	this->SpecularColor = specularColor;

	GenerateMeshData(10.0f, 100, 100);
	//GenRainCameraCone2();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SphereVertex) * meshData.vertices.size();
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
	WCHAR* HLSLFileName = L"Resource/Shader/IBL.hlsl";
	//WCHAR* HLSLFileName = L"Resource/Shader/Sky.hlsl";

	//�ڶ�,����VertexShader����,������VertexShader
	result = D3DX11CompileFromFile(HLSLFileName, NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &VertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// ��ȡָ�������Ϣ�ı���ָ��
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());

		// ��ȡ������Ϣ�ı��ĳ���
		int bufferSize = errorMessage->GetBufferSize();

		std::ofstream fout;

		// ����һ��txt,����д�������Ϣ
		fout.open("shader-error.txt");

		//��txt�ļ�д�������Ϣ
		for (int i = 0; i < bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		// �ر��ļ�
		fout.close();
	}
	Device::Instance().GetDevice()->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
	//����,����PixelShader,������PixelShader
	result = D3DX11CompileFromFile(HLSLFileName, NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &PixelShaderBuffer, &errorMessage, NULL);
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
	matrixBufferDesc.ByteWidth = sizeof(CBufferVertex_SphereForTest);   //�ṹ���С,����Ϊ16�ֽڱ���
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = 0;
	Device::Instance().GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, &mCBMatrixBuffer);


	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE; //�����޳�
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true; //��Ȳü�����
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;// D3D11_FILL_SOLID; //ʵ����Ⱦ
	rasterDesc.FrontCounterClockwise = false; //˳ʱ��
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	Device::Instance().GetDevice()->CreateRasterizerState(&rasterDesc, &mRasterizerState);

	//WCHAR* cubemapFilename = L"Resource/Texture/dds//Converted/hdr.dds";
	WCHAR* cubemapFilename = L"Resource/Texture/snowcube1024.dds";
	result = D3DX11CreateShaderResourceViewFromFile(Device::Instance().GetDevice(), cubemapFilename, 0, 0, &mCubeTexture, 0);
	if (FAILED(result))
	{
		return;
	}
}
bool SphereForTest::init()
{
	return true;
}
void SphereForTest::update(float Delta)
{
	CBufferVertex_SphereForTest cbufferVertex;
	AS3DMATRIX4 tmp = GetWorldTransformMatrix() * Camera::Instance().GetViewProj();
	cbufferVertex.WVProj = tmp.GetTranspose();
	tmp = GetWorldTransformMatrix();
	cbufferVertex.WorldInverseTranspose = tmp.GetInverse().GetTranspose();
	cbufferVertex.WorldMatrix = tmp.GetTranspose();
	cbufferVertex.EyePosition = Camera::Instance().GetEyePos();
	AS3DVECTOR4 SC;
	SC.x = SpecularColor.x;
	SC.y = SpecularColor.y;
	SC.z = SpecularColor.z;
	SC.w = roughness;
	cbufferVertex.SpecularColor = SC;
	Device::Instance().GetContext()->UpdateSubresource(mCBMatrixBuffer, 0, NULL, &cbufferVertex, 0, 0);
}
void SphereForTest::render()
{
	//���ö������벼��
	Device::Instance().GetContext()->IASetInputLayout(mInputLayout);
	Device::Instance().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(SphereVertex);
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
SphereForTest::~SphereForTest()
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
void SphereForTest::GenerateMeshData(float radius, UINT sliceCount, UINT stackCount)
{
	meshData.vertices.clear();
	meshData.indices.clear();

	SphereVertex topVertex(0.0f, +radius, 0.0f);
	SphereVertex bottomVertex(0.0f, -radius, 0.0f);

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

			SphereVertex v;
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

