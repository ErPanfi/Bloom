#include "BaseShader.h"
#include "shaderLoader.h"

HRESULT BaseShader::initializeShadersAndDS(ID3D11Device* pDevice	//the device
										   ,D3D11_TEXTURE2D_DESC* descDepth, D3D11_DEPTH_STENCIL_DESC* descDS, D3D11_DEPTH_STENCIL_VIEW_DESC* descDSV //depth stencil
										   ,D3D11_INPUT_ELEMENT_DESC layout[], UINT layoutLen //input layou
										   ,LPCWSTR vertexFileName //vertex shader
										   ,LPCWSTR pixelFileName //pixel shader
										   )
{
	//output blob
	ID3DBlob* shaderBlob = nullptr;

	//create vertex shader
	HRESULT result = mcg::ShaderLoader::vertexPreBuiltLoad(vertexFileName, pDevice, &mpVertexShader, &shaderBlob);
	if(FAILED(result))
		return result;

	//create input layout
	result = mcg::ShaderLoader::createInputLayout(shaderBlob, pDevice, layout, layoutLen, &mpInputLayout);
	if(FAILED(result))
		return result;

	//release used blob
	shaderBlob -> Release();

	//create pixel shader
	result = mcg::ShaderLoader::pixelPreBuiltLoad(pixelFileName, pDevice, &mpPixelShader);
	if(FAILED(result))
		return result;

	//create depth stencil texture
	result = pDevice -> CreateTexture2D(descDepth, nullptr, &mpDepthStencilTexture);
	if(FAILED(result))
		return result;

	//create depth stencil view
	result = pDevice -> CreateDepthStencilView(mpDepthStencilTexture, descDSV, &mpDepthStencilView);
	if(FAILED(result))
		return result;

	//create depth stencil state
	result = pDevice -> CreateDepthStencilState(descDS, &mpDepthStencilState);

	/*
	if(FAILED(result))
		return result;
	else
	*/
	return result;
}

void BaseShader::prepareContextForRendering(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, DirectX::XMFLOAT4 clearColor)
{
	//imposto render target
	pContext->OMSetRenderTargets(1, &pRenderTargetView, mpDepthStencilView);
	//pulisco render target
	pContext->ClearRenderTargetView(pRenderTargetView, &(clearColor.x));
	//pulisco depth stencil buffer
	pContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//setto primitiva geometria
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//setto depth stencil state
	pContext->OMSetDepthStencilState(mpDepthStencilState, 0);
	//setto input layout
	pContext->IASetInputLayout(mpInputLayout);
	//setto i due shader
	pContext->VSSetShader(mpVertexShader, nullptr, 0);
	pContext->PSSetShader(mpPixelShader, nullptr, 0);
}