#pragma once
#include<d3d11.h>
#include<d3dx11.h>
class ComputeShaderTestModule
{
private:
	ID3D11ComputeShader* mComputeShader;

	ID3D11Buffer*    mBuffer1;
	ID3D11ShaderResourceView* mBufferSRV1;
	ID3D11UnorderedAccessView* mBufferUAV1;

	ID3D11Buffer*    mBuffer2;
	ID3D11ShaderResourceView* mBufferSRV2;
	ID3D11UnorderedAccessView* mBufferUAV2;
public:
	ComputeShaderTestModule();
	~ComputeShaderTestModule();
	void Execute();
};