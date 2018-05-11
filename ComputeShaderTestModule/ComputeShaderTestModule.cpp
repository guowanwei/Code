#include "ComputeShaderTestModule.h"
#include "../Device.h"

#include <stdio.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "D3Dcompiler.h"
#include <vector>
#include <iostream>

struct InputStruct
{
	float x;
	int y;
};
template <class T>
HRESULT CreateStructuredBuffer(ID3D11Device* pd3dDevice, UINT iNumElements, ID3D11Buffer** ppBuffer, ID3D11ShaderResourceView** ppSRV, ID3D11UnorderedAccessView** ppUAV, const T* pInitialData = NULL)
{
	HRESULT hr = S_OK;

	// Create SB
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	//bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bufferDesc.ByteWidth = iNumElements * sizeof(T);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(T);

	D3D11_SUBRESOURCE_DATA bufferInitData;
	ZeroMemory(&bufferInitData, sizeof(bufferInitData));
	bufferInitData.pSysMem = pInitialData;
	hr = pd3dDevice->CreateBuffer(&bufferDesc, (pInitialData) ? &bufferInitData : NULL, ppBuffer);

	// Create SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = iNumElements;
	hr = pd3dDevice->CreateShaderResourceView(*ppBuffer, &srvDesc, ppSRV);

	// Create UAV
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = iNumElements;
	hr = pd3dDevice->CreateUnorderedAccessView(*ppBuffer, &uavDesc, ppUAV);

	return hr;
}

HRESULT CompileComputeShader(LPCWSTR srcFile, LPCSTR entryPoint, ID3D11Device* device, ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !device || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	// We generally prefer to use the higher CS shader profile when possible as CS 5.0 is better performance on 11-class hardware
	LPCSTR profile = (device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DX11CompileFromFile(srcFile, defines, NULL,
		entryPoint, profile,
		flags, 0,NULL, &shaderBlob, &errorBlob,NULL);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}

ComputeShaderTestModule::ComputeShaderTestModule()
{
	//create buffer;
	std::vector<InputStruct> InitData;
	for (int i = 0; i < 100; ++i)
	{
		InputStruct m;
		m.x = i;
		m.y = i;
		InitData.push_back(m);
	}
	CreateStructuredBuffer<InputStruct>(Device::Instance().GetDevice(),100,&mBuffer1,&mBufferSRV1,&mBufferUAV1,&InitData[0]);
	CreateStructuredBuffer<InputStruct>(Device::Instance().GetDevice(),100, &mBuffer2, &mBufferSRV2, &mBufferUAV2, &InitData[0]);
	
	// create compute shader
	// Compile shader
	ID3DBlob *csBlob = nullptr;
	HRESULT hr = CompileComputeShader(L"Resource/Shader/ComputeShader.hlsl", "CSMain", Device::Instance().GetDevice(), &csBlob);
	// Create shader
	mComputeShader = nullptr;
	hr = Device::Instance().GetDevice()->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), nullptr, &mComputeShader);
	csBlob->Release();

	//Device::Instance().GetDevice()->CreateComputeShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
}
ComputeShaderTestModule::~ComputeShaderTestModule()
{
	if (mComputeShader) mComputeShader->Release(); mComputeShader = NULL;
	if (mBufferUAV1) mBufferUAV1->Release(); mBufferUAV1 = NULL;
	if (mBufferSRV1) mBufferSRV1->Release(); mBufferSRV1 = NULL;
	if (mBuffer1) mBuffer1->Release(); mBuffer1 = NULL;


	if (mBufferUAV2) mBufferUAV2->Release(); mBufferUAV2 = NULL;
	if (mBufferSRV2) mBufferSRV2->Release(); mBufferSRV2 = NULL;
	if (mBuffer2) mBuffer2->Release(); mBuffer2 = NULL;
}

void ComputeShaderTestModule::Execute()
{
	Device::Instance().GetContext()->CSSetShaderResources(0, 1, &mBufferSRV1);
	Device::Instance().GetContext()->CSSetUnorderedAccessViews(0, 1, &mBufferUAV2,NULL);
	Device::Instance().GetContext()->CSSetShader(mComputeShader, NULL, 0);
	Device::Instance().GetContext()->Dispatch(10, 1, 1);

	static int x = 0;
	if (x != 0) return;
	x++;
	ID3D11Buffer* pBuffer = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	mBuffer2->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	if (Device::Instance().GetDevice()->CreateBuffer(&desc, NULL, &pBuffer))
	{
		;
	}
	Device::Instance().GetContext()->CopyResource(pBuffer, mBuffer2);

	if (pBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		Device::Instance().GetContext()->Map(pBuffer, 0, D3D11_MAP_READ, 0, &MappedResource);
		InputStruct* p = (InputStruct*)MappedResource.pData;
		for (int i = 0; i < 100; i++)
		{
			std::cout << p[i].y << std::endl;
		}
		Device::Instance().GetContext()->Unmap(pBuffer, 0);

		pBuffer->Release();
		pBuffer = NULL;
	}
}