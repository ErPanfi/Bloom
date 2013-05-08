#pragma once

#include "BaseShader.h"
#include <DirectXMath.h>

class RenderToTexture : public BaseShader
{
protected:
	ID3D11Texture2D*		mpTextureRenderTarget;
	ID3D11RenderTargetView*	mpTextureRenderTargetView;
	ID3D11SamplerState*		mpTextureSamplerState;
	ID3D11ShaderResourceView*	mpTextureShaderResourceView;

public:

	RenderToTexture() 
		:BaseShader()
		,mpTextureRenderTarget(nullptr)
		,mpTextureRenderTargetView(nullptr)
		,mpTextureSamplerState(nullptr)
		,mpTextureShaderResourceView(nullptr)
	{
	}

	~RenderToTexture()
	{
		if(mpTextureRenderTarget)
			mpTextureRenderTarget -> Release();
		if(mpTextureRenderTargetView)
			mpTextureRenderTargetView -> Release();
		if(mpTextureSamplerState)
			mpTextureSamplerState -> Release();
	}

	//attributes getters
	ID3D11Texture2D*		getTextureRenderTarget() const { return mpTextureRenderTarget; }
	ID3D11RenderTargetView*	getTextureRenderTargetView() const { return mpTextureRenderTargetView; }
	ID3D11SamplerState*		getTextureSamplerState() const { return mpTextureSamplerState; }
	
	ID3D11ShaderResourceView* getTextureShaderResourceView() const { return mpTextureShaderResourceView; }

	HRESULT initializeTargetTexture(ID3D11Device* pDevice, D3D11_TEXTURE2D_DESC* pTextureDesc, D3D11_SAMPLER_DESC* pSamplerDesc)
	{
		//creo la texture
		HRESULT result = pDevice -> CreateTexture2D(pTextureDesc, nullptr, &mpTextureRenderTarget);
		if(FAILED(result))
			return result;

		//creo il render target view da associare alla texture - la descrizione matcha quella della texture
		D3D11_RENDER_TARGET_VIEW_DESC descRTV;
		descRTV.Format = pTextureDesc -> Format;
		descRTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		descRTV.Texture2D.MipSlice = 0;
		result = pDevice -> CreateRenderTargetView(mpTextureRenderTarget, &descRTV, &mpTextureRenderTargetView);
		if(FAILED(result))
			return result;

		//descrivo e creo la shader resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
		descSRV.Format = pTextureDesc -> Format;
		descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descSRV.Texture2D.MipLevels = pTextureDesc -> MipLevels;
		descSRV.Texture2D.MostDetailedMip = descSRV.Texture2D.MipLevels -1;

		result = pDevice -> CreateShaderResourceView(mpTextureRenderTarget, &descSRV, &mpTextureShaderResourceView);
		if(FAILED(result))
			return result;

		//creo il sampler state per la texture
		result = pDevice -> CreateSamplerState(pSamplerDesc, &mpTextureSamplerState);
		/*
		if(FAILED(result))
			return result;
		else
		*/
		return result;
	}

	//prepare for rendering overloading
	void prepareContextForRendering(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, DirectX::XMFLOAT4 clearColor)
	{
		BaseShader::prepareContextForRendering(pContext, pRenderTargetView, clearColor);
		pContext -> PSSetSamplers(0, 1, &mpTextureSamplerState);
	}

	void prepareContextForRendering(ID3D11DeviceContext* pContext, DirectX::XMFLOAT4 clearColor)
	{
		prepareContextForRendering(pContext, mpTextureRenderTargetView, clearColor);
	}
};