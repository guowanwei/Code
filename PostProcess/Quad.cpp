#include "Quad.h"
#include <vector>
#include "../Device.h"
#include "../Camera.h"
#include "D3Dcompiler.h"
#include <fstream>
#include "../RenderTarget.h"

struct QuadVertexData
{
	AS3DVECTOR3 Pos;
	AS3DVECTOR3 UV;
	QuadVertexData(AS3DVECTOR3 P, AS3DVECTOR3 U)
		:Pos(P), UV(U)
	{
	}
};
DrawQuad::DrawQuad(const WCHAR* ShaderFilePath)
{

	std::vector<QuadVertexData> Vertices;
	Vertices.push_back(QuadVertexData(AS3DVECTOR3(-1,1,0),AS3DVECTOR3(0,0,0)));// 左上
	Vertices.push_back(QuadVertexData(AS3DVECTOR3(-1, -1, 0), AS3DVECTOR3(0, 1, 0)));// 左下
	Vertices.push_back(QuadVertexData(AS3DVECTOR3(1, 1, 0), AS3DVECTOR3(1, 0, 0)));// 右上

	Vertices.push_back(QuadVertexData(AS3DVECTOR3(-1, -1, 0), AS3DVECTOR3(0, 1, 0)));// 左下
	Vertices.push_back(QuadVertexData(AS3DVECTOR3(1, 1, 0), AS3DVECTOR3(1, 0, 0)));// 右上
	Vertices.push_back(QuadVertexData(AS3DVECTOR3(1, -1, 0), AS3DVECTOR3(1, 1, 0)));// 右下

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(QuadVertexData) * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &Vertices[0];
	Device::Instance().GetDevice()->CreateBuffer(&vbd, &vinitData, &mVertexBuffer);

	

	//compile and create shader
	HRESULT result;
	ID3D10Blob* errorMessage = NULL;
	ID3D10Blob* VertexShaderBuffer = NULL;
	ID3D10Blob* PixelShaderBuffer = NULL;
	D3D10_SHADER_MACRO* Macros;
	
	D3D10_SHADER_MACRO Shader_Macros[] = { NULL,NULL };
	Macros = Shader_Macros;
	
	//const WCHAR* ShaderFilePath = L"Resource/Shader/Lighting.hlsl";
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
		{ "TEXCOORD",0,	 DXGI_FORMAT_R32G32B32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	unsigned int numElements = sizeof(vertexInputLayout) / sizeof(vertexInputLayout[0]);
	result = Device::Instance().GetDevice()->CreateInputLayout(vertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &mInputLayout);
	VertexShaderBuffer->Release();
	VertexShaderBuffer = NULL;
	PixelShaderBuffer->Release();
	PixelShaderBuffer = NULL;

	// sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;// D3D11_FILTER_MIN_MAG_MIP_POINT;
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

	//
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
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

	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	result = Device::Instance().GetDevice()->CreateBlendState(
		&blendDesc,
		&mBlendState
	);
	//depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.StencilEnable = FALSE;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	Device::Instance().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);

}
void DrawQuad::Render(std::vector<RenderTarget*>& InputRenderTargets,std::vector<RenderTarget*>& OutputRenderTargets)
{
	//不写深度不进行深度测试的
	Device::Instance().GetContext()->IASetInputLayout(mInputLayout);
	Device::Instance().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(QuadVertexData);
	UINT offset = 0;
	Device::Instance().GetContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	//Device::Instance().GetContext()->VSSetConstantBuffers(0, 1, &mCBMatrixBufferVS);
	Device::Instance().GetContext()->PSSetSamplers(0, 1, &mSamplerState);

	//设置VertexShader和PixelShader
	Device::Instance().GetContext()->VSSetShader(mVertexShader, NULL, 0);
	Device::Instance().GetContext()->PSSetShader(mPixelShader, NULL, 0);
	Device::Instance().GetContext()->RSSetState(mRasterizerState);
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Device::Instance().GetContext()->OMSetBlendState(mBlendState, blendFactor, 0xffffffff);
	Device::Instance().GetContext()->OMSetDepthStencilState(mDepthStencilState, 0);
	
	if (OutputRenderTargets.size() > 0)
	{
		for (int i = 0; i < OutputRenderTargets.size(); ++i)
		{
			Device::Instance().ClearRenderTarget(OutputRenderTargets[i]->GetRenderTargetView());
		}
		Device::Instance().SetRenderTargets(OutputRenderTargets);
	}
	else
	{
		Device::Instance().DrawOnScreenFinally();
	}
	//操了他的妈了，PSSetShaderResources 一直绑定不成功，msdn中给的解释
	//所以这个放在 SetRenderTarget之后。。。。。。。。
	/*If an overlapping resource view is already bound to an output slot, such as a rendertarget, then this API will fill the destination shader resource slot with NULL.
	*/
	for (int i = 0; i < InputRenderTargets.size(); ++i)
	{
		Device::Instance().GetContext()->PSSetShaderResources(i, 1, &(InputRenderTargets[i]->GetShaderResourceView()));
	}
	Device::Instance().GetContext()->Draw(6, 0);
}

DrawQuad::~DrawQuad()
{
	if (mVertexBuffer)
	{
		mVertexBuffer->Release(); 
		mVertexBuffer = NULL;
	}

	if (mInputLayout)
	{
		mInputLayout->Release(); 
		mInputLayout = NULL;
	}
	if (mVertexShader)
	{
		mVertexShader->Release();
		mVertexShader = NULL;
	}
	if (mPixelShader)
	{
		mPixelShader->Release();
		mPixelShader = NULL;
	}

	if (mSamplerState)
	{
		mSamplerState->Release();
		mSamplerState = NULL;
	}


	if (mRasterizerState)
	{
		mRasterizerState->Release();
		mRasterizerState = NULL;
	}
	if (mBlendState)
	{
		mBlendState->Release(); mBlendState = NULL;
	}
	if (mDepthStencilState)
	{
		mDepthStencilState->Release();
		mDepthStencilState = NULL;
	}
}