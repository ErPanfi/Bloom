//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "BloomProgram.h"
#include "timer.h"
#include "shaderLoader.h"
#include "buffer.h"
#include "textDrawer.h"


#pragma comment(lib, "DXUtils.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "DirectXTK.lib")



using namespace DirectX;


BloomProgram::BloomProgram(const HINSTANCE hInstance, const int nCmdShow)
	: mcg::SampleProgram(hInstance, nCmdShow)
	, mVertexBuffer(nullptr)
	, mIndexBuffer(nullptr)
	, mPlaneVertexBuffer(nullptr)
	, mPlaneIndexBuffer(nullptr)
	, mDepthStencilState(nullptr)
	, mMaterialCBuffer(nullptr)
	, mPlaneMaterialCBuffer(nullptr)
	, mTranfCBuffer(nullptr)
	, mLightsCBuffer(nullptr)
	, pauseLightsRotation(false)
	, brightPass(false)
	, cPos(3.0f)
	, shiftPos(0.0f)
	, cThresh(0.5f)
	, mpBrightPassShader(nullptr)
	, mpPhongShader(nullptr)
	, mpTextureMergeShader(nullptr)
{
	for(int i = 0; i < MAX_KEY; ++i)
		keys[i] = false;
}

HRESULT BloomProgram::initializeResources()
{
	HRESULT result = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	mcg::PositionNormal vertices[] =
    {
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ) },

        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ) },
    };

	unsigned int indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

	// Creazione del vertex e index buffer per il cubo
	result = mcg::createVertexBuffer(mPd3dDevice, vertices, 24, D3D11_USAGE_DEFAULT, &mVertexBuffer);
    if( FAILED( result ) )
        return result;

	result = mcg::createIndexBuffer(mPd3dDevice, indices, 36, D3D11_USAGE_DEFAULT, &mIndexBuffer);
    if( FAILED( result ) )
        return result;

	// Creazione del costant buffer per le trasformazioni.
	mTransforms.view = mCamera.getViewMatrix();
	mTransforms.cameraPosition = mCamera.getCameraPositionFlaot4();
	mTransforms.world = XMMatrixIdentity();
	mTransforms.projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), getWindowAspectRatio(), 0.1f, 100.0f);

	result = mcg::createCostantBuffer(mPd3dDevice, &mTransforms, D3D11_USAGE_DEFAULT, &mTranfCBuffer);
    if( FAILED( result ) )
        return result;

	// Creazione del costant buffer per il materiale.
	mMaterial.ambiental = XMFLOAT4(0.1f, 0.3f, 0.3f, 1.0f);
	mMaterial.diffusive = XMFLOAT4(0.2f, 0.5f, 0.5f, 1.0f);
	mMaterial.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mMaterial.shininess = 128;

	mPlaneMaterial.ambiental = XMFLOAT4(0.3f, 0.3f, 0.1f, 1.0f);
	mPlaneMaterial.diffusive = XMFLOAT4(0.5f, 0.5f, 0.2f, 1.0f);
	mPlaneMaterial.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mPlaneMaterial.emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mPlaneMaterial.shininess = 128;

	result = mcg::createCostantBuffer(mPd3dDevice, &mMaterial, D3D11_USAGE_DEFAULT, &mMaterialCBuffer);
    if( FAILED( result ) )
        return result;

	//creazione constant buffer per soglia luminosit�
	BrightThresh thresh;
	thresh.threshold = cThresh;

	result = mcg::createCostantBuffer(mPd3dDevice, &thresh, D3D11_USAGE_DEFAULT, &mBrightThreshBuffer);
    if( FAILED( result ) )
        return result;

	// Creazione di un constant buffer con le luci.
	// Numero di luci nella scena.
	mLights.usedLights = 5;

	// Prima luce
	mLights.position[0] = XMFLOAT4(2.0f, 0.0f, 0.0f, 1.0f);
	mLights.color[0] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mLights.ambient[0] = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mLights.attenuation[0] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	// Seconda luce
	mLights.position[1] = XMFLOAT4(2.0f, -2.0f, 0.0f, 1.0f);
	mLights.color[1] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mLights.ambient[1] = XMFLOAT4(0.3f, 0.1f, 0.1f, 1.0f);
	mLights.attenuation[1] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	//sysref rosso
	mLights.position[2] = XMFLOAT4(cPos, 0.0f, shiftPos, 1.0f);
	mLights.color[2] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mLights.ambient[2] = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mLights.attenuation[2] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	//sysref verde
	mLights.position[3] = XMFLOAT4(shiftPos, cPos, 0.0f, 1.0f);
	mLights.color[3] = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	mLights.ambient[3] = XMFLOAT4(0.3f, 0.1f, 0.1f, 1.0f);
	mLights.attenuation[3] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	//sysref blu
	mLights.position[4] = XMFLOAT4(0.0f, shiftPos, cPos, 1.0f);
	mLights.color[4] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	mLights.ambient[4] = XMFLOAT4(0.3f, 0.1f, 0.1f, 1.0f);
	mLights.attenuation[4] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	result = mcg::createCostantBuffer(mPd3dDevice, &mLights, D3D11_USAGE_DEFAULT, &mLightsCBuffer);
    if( FAILED( result ) )
        return result;
	/*
	// Creazione degli shader per Gourad
	ID3DBlob* vsGouradBlob = nullptr;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./04-IlluminazioneShading/GouradShadingVS.cso", mPd3dDevice, &mGouradVS, &vsGouradBlob);
	if( FAILED( result ) )
        return result;

	result = mcg::ShaderLoader::createInputLayout(vsGouradBlob, mPd3dDevice, layout, 2, &mGouradInputLayout);
	if( FAILED( result ) )
        return result;

	vsGouradBlob->Release();
	
	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./04-IlluminazioneShading/GouradShadingPS.cso", mPd3dDevice, &mGouradPS);
	if( FAILED( result ) )
        return result;
	*/

	//descrizioni dei tre componenti del depth buffer
	RECT rc;
    GetClientRect( mHWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;;
	descDepth.SampleDesc.Count = 1;	//no multisampling
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = (1 > 1)? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;

	D3D11_DEPTH_STENCIL_DESC descDS;
	descDS.DepthEnable = true;
	descDS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDS.DepthFunc = D3D11_COMPARISON_LESS;
	descDS.StencilEnable = false;


	
	//creazione degli shader per la gestione delle texture
	/*
	//vertex shader banale, per gestire le texture
	ID3DBlob* textureVSBlob = nullptr;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./TextureTrivialVS.cso", mPd3dDevice, &mPhongVS, &textureVSBlob);
	if( FAILED( result ) )
        return result;
	
	//descrizione input layout per il VS
	D3D11_INPUT_ELEMENT_DESC texInputLayoutDesc[] = {{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }};
	result = mcg::ShaderLoader::createInputLayout(textureVSBlob, mPd3dDevice, texInputLayoutDesc, 1, &mBrightPassPSInputLayout);
	if( FAILED( result ) )
        return result;

	textureVSBlob->Release();


	//bright pass PS
	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./BrightPassPS.cso", mPd3dDevice, &mBrightPassPS);
	if( FAILED( result ) )
        return result;
	*/
	//descrizione e creazione della texture di appoggio
	D3D11_TEXTURE2D_DESC			textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	//descrizione input layout per il VS
	D3D11_INPUT_ELEMENT_DESC texInputLayoutDesc[] = 
	{
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	PosTexCoords texVertexes[] = 
	{
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(-1.0f, -1.0f)},
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, -1.0f)},
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(-1.0f, 1.0f)},
	};

	result = mcg::createVertexBuffer(mPd3dDevice, texVertexes, 4, D3D11_USAGE_DEFAULT, &mpTextureVertexBuffer);
    if( FAILED( result ) )
        return result;

	unsigned int texIndexes[] = 
	{
		0, 2, 1,
		2, 0, 3
	};

	result = mcg::createIndexBuffer(mPd3dDevice, indices, 36, D3D11_USAGE_DEFAULT, &mIndexBuffer);
    if( FAILED( result ) )
        return result;


	//descrizione del sampler state
	D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Creazione degli shader per Phong
	/*
	ID3DBlob* phongShaderBlob = nullptr;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./PhongShadingVS.cso", mPd3dDevice, &mPhongVS, &phongShaderBlob);
	if( FAILED( result ) )
        return result;

	result = mcg::ShaderLoader::createInputLayout(phongShaderBlob, mPd3dDevice, layout, 2, &mPhongInputLayout);
	if( FAILED( result ) )
        return result;

	phongShaderBlob->Release();

	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./PhongShadingPS.cso", mPd3dDevice, &mPhongPS);
	if( FAILED( result ) )
        return result;
	*/
	mpPhongShader = new RenderToTexture();
	result = mpPhongShader -> initializeShadersAndDS(mPd3dDevice, &descDepth, &descDS, &descDSV, layout, 2, L"./PhongShadingVS.cso", L"./PhongShadingPS.cso");
	if(FAILED(result))
		return result;

	result = mpPhongShader -> initializeTargetTexture(mPd3dDevice, &textureDesc, &sampDesc);
	if(FAILED(result))
		return result;

	//bright pass shader
	mpBrightPassShader = new RenderToTexture();

	result = mpBrightPassShader -> initializeShadersAndDS(mPd3dDevice, &descDepth, &descDS, &descDSV, texInputLayoutDesc, 2, L"./TextureTrivialVS.cso", L"./BrightPassPS.cso");
	if( FAILED( result ) )
        return result;

	result = mpBrightPassShader -> initializeTargetTexture(mPd3dDevice, &textureDesc, &sampDesc);
	if( FAILED( result ) )
        return result;

	//texture merge shader
	mpTextureMergeShader = new RenderToTexture();

	result = mpTextureMergeShader -> initializeShadersAndDS(mPd3dDevice, &descDepth, &descDS, &descDSV, texInputLayoutDesc, 2, L"./TextureTrivialVS.cso", L"./TextureMergePS.cso");
	if( FAILED( result ) )
        return result;

	result = mpTextureMergeShader -> initializeTargetTexture(mPd3dDevice, &textureDesc, &sampDesc);
	if( FAILED( result ) )
        return result;

	//////////////////
	mLightSphere = std::unique_ptr<DirectX::GeometricPrimitive>(GeometricPrimitive::CreateSphere(mPd3dDeviceContext, 1.0f, 10));
	mTextDrawer = new mcg::TextDrawer(mPd3dDeviceContext);
	mArialFont = new mcg::TextFont(mPd3dDevice, L"./arial16.spritefont");
	mTextDrawer->setColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	mTextDrawer->setPosition(DirectX::XMFLOAT2(10.0f, 10.0f));
	//////////////////

	return result;
}

void BloomProgram::preRender()
{
	static mcg::Timer timer;
	float fSec = timer.elapsedSecF();
	timer.start();

	// Aggiorniamo la camera.
	// Input
	XMFLOAT4 cameraRotationAxe(0.0f, 0.0f, 0.0f, 0.0f);
	if(keys[ROTATE_CAMERA_LEFT])
		mCamera.yRotation(fSec);
	if(keys[ROTATE_CAMERA_RIGHT])
		mCamera.yRotation(-fSec);
	if(keys[ROTATE_CAMERA_UP])
		mCamera.xRotation(-fSec);
	if(keys[ROTATE_CAMERA_DOWN])
		mCamera.xRotation(fSec);
	if(keys[MOVE_CAMERA_FOREWARD])
		mCamera.translate(-fSec);
	if(keys[MOVE_CAMERA_BACKWARD])
		mCamera.translate(fSec);

	mTransforms.view = mCamera.getViewMatrix();
	mTransforms.cameraPosition = mCamera.getCameraPositionFlaot4();

	mPd3dDeviceContext->UpdateSubresource( mTranfCBuffer, 0, nullptr, &mTransforms, 0, 0 );

	// Aggiorniamo le luci.
	if(keys[CPOS_INC] || keys[CPOS_DEC])
	{

		if(keys[CPOS_INC])
			shiftPos = 0.001f;
		if(keys[CPOS_DEC])
			shiftPos = -0.001f;

		//sysref rosso
		XMMATRIX lightRotation = XMMatrixRotationY(shiftPos);
		XMVECTOR newLightPosition = XMVector4Transform(XMLoadFloat4(&mLights.position[2]), lightRotation);
		XMStoreFloat4(&mLights.position[2], newLightPosition);
		//sysref verde
		lightRotation = XMMatrixRotationZ(shiftPos);
		newLightPosition = XMVector4Transform(XMLoadFloat4(&mLights.position[3]), lightRotation);
		XMStoreFloat4(&mLights.position[3], newLightPosition);
		//sysref blu
		lightRotation = XMMatrixRotationX(shiftPos);
		newLightPosition = XMVector4Transform(XMLoadFloat4(&mLights.position[4]), lightRotation);
		XMStoreFloat4(&mLights.position[4], newLightPosition);
	}

	if(!pauseLightsRotation)
	{
		XMMATRIX lightRotation = XMMatrixRotationZ(fSec);
		XMVECTOR newLightPosition = XMVector4Transform(XMLoadFloat4(&mLights.position[0]), lightRotation);
		XMStoreFloat4(&mLights.position[0], newLightPosition);

		XMVECTOR lightRotationAxe = {1.0f, 1.0f, 0.0f, 0.0f};
		lightRotation = XMMatrixRotationAxis(lightRotationAxe, 1.2f * fSec);
		newLightPosition = XMVector4Transform(XMLoadFloat4(&mLights.position[1]), lightRotation);
		XMStoreFloat4(&mLights.position[1], newLightPosition);
	}

	if(keys[CPOS_INC] || keys[CPOS_DEC] || !pauseLightsRotation)
		mPd3dDeviceContext->UpdateSubresource( mLightsCBuffer, 0, nullptr, &mLights, 0, 0 );

	if(keys[BTHRESH_DEC] || keys[BTHRESH_INC])
	{
		if(keys[BTHRESH_DEC])
			cThresh -= 0.0001f;
		else
			cThresh += 0.0001f;

		BrightThresh thresh;
		thresh.threshold = cThresh;

		mPd3dDeviceContext->UpdateSubresource(mBrightThreshBuffer, 0, nullptr, &thresh, 0, 0);
	}
}

void BloomProgram::render()
{
	XMFLOAT4 clearColor(0.0f, 0.0f, 0.0f, 1.0f );	//opaque black

	//primo passo: renderizziamo la scena sulla texture
	/*
	mPd3dDeviceContext->OMSetRenderTargets(1, &mBaseTextureRenderTargetView, mPDepthStencilView);
	// Pulizia del render target.
    mPd3dDeviceContext->ClearRenderTargetView( mBaseTextureRenderTargetView, &(clearColor.x) );
	// Pulizia del depth buffer
	mPd3dDeviceContext->ClearDepthStencilView(mPDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Settiamo la topologia della nostra geometria.
	mPd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mPd3dDeviceContext->OMSetDepthStencilState(mDepthStencilState, 0);

	// Settiamo l'input layout.
	mPd3dDeviceContext->IASetInputLayout(mPhongInputLayout);
	// Settiamo gli shader da utilizzare
	mPd3dDeviceContext->VSSetShader(mPhongVS, nullptr, 0);
	mPd3dDeviceContext->PSSetShader(mPhongPS, nullptr, 0);
	*/
	if(brightPass)
		mpPhongShader -> prepareContextForRendering(mPd3dDeviceContext, clearColor);	
	else
		mpPhongShader -> prepareContextForRendering(mPd3dDeviceContext,mPRenderTargetView, clearColor);

	//Settiamo i constant buffer da utilizzare.
	mPd3dDeviceContext->VSSetConstantBuffers(0, 1, &mTranfCBuffer);
	mPd3dDeviceContext->PSSetConstantBuffers(0, 1, &mMaterialCBuffer);
	mPd3dDeviceContext->PSSetConstantBuffers(1, 1, &mLightsCBuffer);

	// Disegno del cubo.
	// Settiamo il vertex buffer da renderizzare.
	UINT stride = sizeof(mcg::PositionNormal);
	UINT offset = 0;
	mPd3dDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mPd3dDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	mPd3dDeviceContext->DrawIndexed(36, 0, 0);

	mPd3dDeviceContext->PSSetConstantBuffers(0, 1, &mPlaneMaterialCBuffer);
	mPd3dDeviceContext->IASetVertexBuffers(0, 1, &mPlaneVertexBuffer, &stride, &offset);
	mPd3dDeviceContext->IASetIndexBuffer(mPlaneIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	mPd3dDeviceContext->DrawIndexed(6, 0, 0);

	for(int i = 0; i < mLights.usedLights; ++i)
	{
		XMMATRIX lightWorld = XMMatrixTranslation(mLights.position[i].x, mLights.position[i].y, mLights.position[i].z);
		lightWorld = XMMatrixMultiply(XMMatrixScaling(0.3f, 0.3f, 0.3f), lightWorld);
		mLightSphere->Draw(lightWorld, mTransforms.view, mTransforms.projection, XMLoadFloat4(&mLights.color[i]));
	}

	//disegno della texture di base concluso

	//rendering aggiuntivi?
	if(brightPass)
	{
		//secondo passo: applicazione brightpass alla texture di base
		mpBrightPassShader -> prepareContextForRendering(mPd3dDeviceContext, clearColor);

		//Settiamo i constant buffer da utilizzare.
		mPd3dDeviceContext->PSSetConstantBuffers(0,1,&mBrightThreshBuffer);
		//settiamo shader resource per l'utilizzo della texture precedente
		ID3D11ShaderResourceView* srvPtr = mpPhongShader->getTextureShaderResourceView();
		mPd3dDeviceContext->PSSetShaderResources(0, 1, &srvPtr);
		//disegniamo la texture
		mPd3dDeviceContext->IASetIndexBuffer(mpTextureIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		mPd3dDeviceContext->DrawIndexed(6, 0, 0);
		//fine secondo passo

		//terzo passo: texture merging
		//specifichiamo manualmente il render target, in modo da istruirlo per renderizzare a schermo
		mpTextureMergeShader -> prepareContextForRendering(mPd3dDeviceContext, mPRenderTargetView, clearColor);
		//settiamo i due shader resource per l'utilizzo delle texture precedenti
		srvPtr = mpPhongShader->getTextureShaderResourceView();
		mPd3dDeviceContext->PSSetShaderResources(0, 1, &srvPtr);
		srvPtr = mpBrightPassShader->getTextureShaderResourceView();
		mPd3dDeviceContext->PSSetShaderResources(1, 1, &srvPtr);
		//disegniamo la texture
		mPd3dDeviceContext->IASetIndexBuffer(mpTextureIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		mPd3dDeviceContext->DrawIndexed(6, 0, 0);
		//fine terzo passo
	}

	/////////////////////////////
	//finale: rendering della texture sul render target del monitor

	// Settiamo il render target corrente.
	mPd3dDeviceContext->OMSetRenderTargets(1, &mPRenderTargetView, mPDepthStencilView);

	mTextDrawer->beginDraw();
	if(brightPass)
		mTextDrawer->drawText(*mArialFont, L"Bright pass is " + std::to_wstring(cThresh));// + L" - ( " + std::to_wstring(1000.0 / tickSecs) + L" FPS)");
	else
		mTextDrawer->drawText(*mArialFont, L"Bright pass is OFF (" + std::to_wstring(cThresh) + L")");// - ( " + std::to_wstring(1000.0 / tickSecs) + L" FPS)");
	mTextDrawer->endDraw();
	////////////////////////////


	mPSwapChain->Present( 0, 0 );
}

void BloomProgram::keyPressed(WPARAM key, LPARAM param)
{
	switch(key)
	{
		case VK_SPACE:	brightPass = !brightPass; break;
		case 0x50:		pauseLightsRotation = !pauseLightsRotation; break; // P

		case 0x41: keys[ROTATE_CAMERA_LEFT]		= true; break; // A
		case 0x44: keys[ROTATE_CAMERA_RIGHT]	= true; break; // D
		case 0x57: keys[ROTATE_CAMERA_UP]		= true; break; // W
		case 0x53: keys[ROTATE_CAMERA_DOWN]		= true; break; // S
		case 0x51: keys[MOVE_CAMERA_FOREWARD]	= true; break; // Q
		case 0x45: keys[MOVE_CAMERA_BACKWARD]	= true; break; // E
		case 0x47: keys[CPOS_DEC]				= true; break; // G
		case 0x54: keys[CPOS_INC]				= true; break; // T
		case 0x59: keys[BTHRESH_INC]			= true; break; // Y
		case 0x48: keys[BTHRESH_DEC]			= true; break; // H
		default: SampleProgram::keyPressed(key, param);
	}
}

void BloomProgram::keyReleased(WPARAM key, LPARAM param)
{
	switch(key)
	{
		case 0x41: keys[ROTATE_CAMERA_LEFT]		= false; break; // A
		case 0x44: keys[ROTATE_CAMERA_RIGHT]	= false; break; // D
		case 0x57: keys[ROTATE_CAMERA_UP]		= false; break; // W
		case 0x53: keys[ROTATE_CAMERA_DOWN]		= false; break; // S
		case 0x51: keys[MOVE_CAMERA_FOREWARD]	= false; break; // Q
		case 0x45: keys[MOVE_CAMERA_BACKWARD]	= false; break; // E
		case 0x47: keys[CPOS_DEC]				= false; break; // G
		case 0x54: keys[CPOS_INC]				= false; break; // T
		case 0x59: keys[BTHRESH_INC]			= false; break; // Y
		case 0x48: keys[BTHRESH_DEC]			= false; break; // H
		default: SampleProgram::keyReleased(key, param);
	}
}

void BloomProgram::cleanResouces()
{
	if(mVertexBuffer)
		mVertexBuffer->Release();

	if(mIndexBuffer)
		mIndexBuffer->Release();

	if(mPlaneVertexBuffer)
		mPlaneVertexBuffer->Release();

	if(mPlaneIndexBuffer)
		mPlaneIndexBuffer->Release();

	if(mDepthStencilState)
		mDepthStencilState->Release();

	if(mMaterialCBuffer)
		mMaterialCBuffer->Release();

	if(mPlaneMaterialCBuffer)
		mPlaneMaterialCBuffer->Release();

	if(mTranfCBuffer)
		mTranfCBuffer->Release();

	if(mLightsCBuffer)
		mLightsCBuffer->Release();

	if(mBrightThreshBuffer)
		mBrightThreshBuffer->Release();

	if(mpPhongShader)
		delete mpPhongShader;

	if(mpBrightPassShader)
		delete mpBrightPassShader;

	if(mpTextureMergeShader)
		delete mpTextureMergeShader;

	if(mpTextureVertexBuffer)
		mpTextureVertexBuffer -> Release();

	if(mpTextureIndexBuffer)
		mpTextureIndexBuffer -> Release();

}