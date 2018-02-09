#include "StdAfx.h"
#include"DemoBase.h"
//#include "DirectXMath.h"

Dx11DemoBase::Dx11DemoBase( ) 
	: driverType_( D3D_DRIVER_TYPE_NULL ), featureLevel_( D3D_FEATURE_LEVEL_11_0 )
	,d3dDevice_( 0 ), d3dContext_( 0 ), swapChain_( 0 ), backBufferTarget_( 0 )
	, mDepthStencilBuffer(0), mDepthStencilView(0)
	, mVertexBuffer(0), mIndexBuffer(0)
	, mVertexShader(0), mPixelShader(0)
	, mInputLayout(0), mCBMatrixBuffer(0)
{}
Dx11DemoBase::~Dx11DemoBase( )
{
	Shutdown( );
}
bool Dx11DemoBase::LoadContent( )
{
	// Override with demo specifics, if any...

	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), Colors::White },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), Colors::Black },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), Colors::Red },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), Colors::Blue },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), Colors::Yellow },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), Colors::Cyan },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), Colors::Magenta }
	};
	//创建vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	d3dDevice_->CreateBuffer(&vbd, &vinitData, &mVertexBuffer);

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
	HRESULT a = d3dDevice_->CreateBuffer(&ibd, &iinitData, &mIndexBuffer);


	//编译shader
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* VertexShaderBuffer;
	ID3D10Blob* PixelShaderBuffer;

	//第一,初始化参数
	errorMessage = NULL;
	VertexShaderBuffer = NULL;
	PixelShaderBuffer = NULL;
	WCHAR* VSFileName = L"MyShader/MyShader.fx";

	//第二,编译VertexShader代码,并创建VertexShader
	result = D3DX11CompileFromFile(VSFileName, NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &VertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		;
	}
	d3dDevice_->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);

	WCHAR* PSFileName = L"MyShader/MyShader.fx";
	//第三,编译PixelShader,并创建PixelShader
	result = D3DX11CompileFromFile(PSFileName, NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &PixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		;
	}
	d3dDevice_->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);
	
	//输入布局！
	D3D11_INPUT_ELEMENT_DESC vertexInputLayout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int numElements = sizeof(vertexInputLayout) / sizeof(vertexInputLayout[0]);         //布局数量

	d3dDevice_->CreateInputLayout(vertexInputLayout, numElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &mInputLayout);
	
	//释放VertexShaderBuffer和PixelShaderBuffer
	VertexShaderBuffer->Release();
	VertexShaderBuffer = NULL;
	PixelShaderBuffer->Release();
	PixelShaderBuffer = NULL;


	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixBufferDesc.ByteWidth = sizeof(XMMATRIX);   //结构体大小,必须为16字节倍数
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = 0;

	d3dDevice_->CreateBuffer(&matrixBufferDesc, NULL, &mCBMatrixBuffer);

	return true;
}
void Dx11DemoBase::UnloadContent( )
{
	// Override with demo specifics, if any...
}
void Dx11DemoBase::Shutdown( )
{
	UnloadContent( );
	if( backBufferTarget_ ) backBufferTarget_->Release( );
	if( swapChain_ ) swapChain_->Release( );
	if( d3dContext_ ) d3dContext_->Release( );
	if( d3dDevice_ ) d3dDevice_->Release( );

	if (mDepthStencilBuffer) mDepthStencilBuffer->Release();
	if (mDepthStencilView) mDepthStencilView->Release();

	if (mVertexBuffer) mVertexBuffer->Release();
	if (mIndexBuffer) mIndexBuffer->Release();

	if (mVertexShader) mVertexShader->Release();
	if (mPixelShader) mPixelShader->Release();
	if (mInputLayout) mInputLayout->Release();
	if (mCBMatrixBuffer) mCBMatrixBuffer->Release();

	backBufferTarget_ = 0;
	swapChain_ = 0;
	d3dContext_ = 0;
	d3dDevice_ = 0;

	mDepthStencilBuffer = 0;
	mDepthStencilView = 0;
	mVertexBuffer = 0;
	mIndexBuffer = 0;

	mVertexShader = 0;
	mPixelShader = 0;
	mInputLayout = 0;

	mCBMatrixBuffer = 0;
}

bool Dx11DemoBase::Initialize( HINSTANCE hInstance, HWND hwnd )
{
	hInstance_ = hInstance;
	hwnd_ = hwnd;
	RECT dimensions;
	GetClientRect( hwnd, &dimensions );
	unsigned int width = dimensions.right - dimensions.left;
	unsigned int height = dimensions.bottom - dimensions.top;
	D3D_DRIVER_TYPE driverTypes[] =
	{
	D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
	D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
	};
	unsigned int totalDriverTypes = ARRAYSIZE( driverTypes );
	D3D_FEATURE_LEVEL featureLevels[] =
	{
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0
	};
	unsigned int totalFeatureLevels = ARRAYSIZE( featureLevels );
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	unsigned int creationFlags = 0;
	//#ifdef _DEBUG
	//creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	//#endif
	HRESULT result;
	unsigned int driver = 0;
	for( driver = 0; driver < totalDriverTypes; ++driver )
	{
	result = D3D11CreateDeviceAndSwapChain( 0, driverTypes[driver], 0, creationFlags,
	featureLevels, totalFeatureLevels,
	D3D11_SDK_VERSION, &swapChainDesc, &swapChain_,
	&d3dDevice_, &featureLevel_, &d3dContext_ );
	if( SUCCEEDED( result ) )
	{
	driverType_ = driverTypes[driver];
	break;
	}
	}
	if( FAILED( result ) )
	{
	//DXTRACE_MSG( "Failed to create the Direct3D device!" );
	return false;
	}
	ID3D11Texture2D* backBufferTexture;
	result = swapChain_->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBufferTexture );
	if( FAILED( result ) )
	{
	//DXTRACE_MSG( "Failed to get the swap chain back buffer!" );
	return false;
	}
	result = d3dDevice_->CreateRenderTargetView( backBufferTexture, 0, &backBufferTarget_ );
	if( backBufferTexture )
	backBufferTexture->Release( );
	if( FAILED( result ) )
	{
	//DXTRACE_MSG( "Failed to create the render target view!" );
	return false;
	}

	// add depth stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	
	d3dDevice_->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	d3dDevice_->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);



	d3dContext_->OMSetRenderTargets( 1, &backBufferTarget_, mDepthStencilView);
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	d3dContext_->RSSetViewports( 1, &viewport );
	return LoadContent( );
}