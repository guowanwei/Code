#include "stdafx.h"
#include "RenderQuard.h"
RenderQuard::RenderQuard()
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



	result = D3DX11CompileFromFile(L"Resource/Shader/Common.hlsl", Macros, NULL, "GBufferPS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &PixelShaderBuffer, &errorMessage, NULL);
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
	Device::Instance().GetDevice()->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &mGBufferPixelShader);

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
