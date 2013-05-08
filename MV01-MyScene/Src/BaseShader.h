#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class BaseShader
{
protected:
	ID3D11InputLayout*			mpInputLayout;
	ID3D11VertexShader*			mpVertexShader;
	ID3D11PixelShader*			mpPixelShader;
	ID3D11Texture2D*			mpDepthStencilTexture;
	ID3D11DepthStencilState*	mpDepthStencilState;
	ID3D11DepthStencilView*		mpDepthStencilView;

public:

	//ctors and dtors
	BaseShader()
		:mpInputLayout(nullptr)
		,mpVertexShader(nullptr)
		,mpPixelShader(nullptr)
		,mpDepthStencilTexture(nullptr)
		,mpDepthStencilState(nullptr)
		,mpDepthStencilView(nullptr)		
	{
	}

	~BaseShader()
	{
		if(mpInputLayout)
			mpInputLayout->Release();

		if(mpVertexShader)
			mpVertexShader->Release();

		if(mpPixelShader)
			mpPixelShader->Release();
	}

	//attributes getter
	ID3D11InputLayout*			getInputLayout() const			{ return mpInputLayout;			}
	ID3D11VertexShader*			getVertexShader() const			{ return mpVertexShader;		}
	ID3D11PixelShader*			getPixelShader() const			{ return mpPixelShader;			}
	ID3D11Texture2D*			getDepthStencilTexture() const	{ return mpDepthStencilTexture; }
	ID3D11DepthStencilState*	getDepthStencilState() const	{ return mpDepthStencilState;	}
	ID3D11DepthStencilView*		getDepthStencilView() const		{ return mpDepthStencilView;	}

	//attributes setter for manual initialization
	void setInputLayout(ID3D11InputLayout* newValue) 
	{ 
		if(mpInputLayout)
			mpInputLayout -> Release();

		mpInputLayout = newValue; 
	}

	void setVertexShader(ID3D11VertexShader* newValue) 
	{ 
		if(mpVertexShader)
			mpVertexShader -> Release();

		mpVertexShader = newValue; 
	}

	void setPixelShader(ID3D11PixelShader* newValue) 
	{ 
		if(mpPixelShader)
			mpPixelShader -> Release();

		mpPixelShader = newValue; 
	}

	void setDSTexture(ID3D11Texture2D* newValue) 
	{ 
		if(mpDepthStencilTexture)
			mpDepthStencilTexture -> Release();

		mpDepthStencilTexture = newValue; 
	}

	void setDSState(ID3D11DepthStencilState* newValue) 
	{ 
		if(mpDepthStencilState)
			mpDepthStencilState -> Release();

		mpDepthStencilState = newValue; 
	}

	void setDSView(ID3D11DepthStencilView* newValue) 
	{ 
		if(mpDepthStencilView)
			mpDepthStencilView -> Release();

		mpDepthStencilView = newValue; 
	}


	//init the shaders
	HRESULT initializeShadersAndDS(ID3D11Device* pDevice, D3D11_TEXTURE2D_DESC* descDepth, D3D11_DEPTH_STENCIL_DESC* descDS, D3D11_DEPTH_STENCIL_VIEW_DESC* descDSV, D3D11_INPUT_ELEMENT_DESC layout[], UINT layoutLen, LPCWSTR vertexFileName, LPCWSTR pixelFileName);
	//prepare for rendering
	void prepareContextForRendering(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, DirectX::XMFLOAT4 clearColor);

};