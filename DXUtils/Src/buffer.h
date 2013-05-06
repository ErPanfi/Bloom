//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <vector>

namespace mcg
{
	template <typename T>
	HRESULT createVertexBuffer(ID3D11Device* device, const T* data, const size_t dataSize, const D3D11_USAGE usage, ID3D11Buffer** outBuffer)
	{
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = usage;
		bufferDesc.ByteWidth = sizeof( T ) * dataSize;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;

		HRESULT result = device->CreateBuffer( &bufferDesc, &initData, outBuffer );
		return result;
	}

	template <typename T>
	HRESULT createIndexBuffer(ID3D11Device* device, const T* data, const size_t dataSize, const D3D11_USAGE usage, ID3D11Buffer** outBuffer)
	{
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = usage;
		bufferDesc.ByteWidth = sizeof( T ) * dataSize;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;

		HRESULT result = device->CreateBuffer( &bufferDesc, &initData, outBuffer );
		return result;
	}

	template <typename T>
	HRESULT createVertexBuffer(ID3D11Device* device, const std::vector<T>& data, const D3D11_USAGE usage, ID3D11Buffer** outBuffer)
	{
		return createVertexBuffer(device, data.data(), data.size(), usage, outBuffer);
	}

	template <typename T>
	HRESULT createIndexBuffer(ID3D11Device* device, const std::vector<T>& data, const D3D11_USAGE usage, ID3D11Buffer** outBuffer)
	{
		return createIndexBuffer(device, data.data(), data.size(), usage, outBuffer);
	}

	template <typename T>
	HRESULT createCostantBuffer(ID3D11Device* device, const T* data, const D3D11_USAGE usage, ID3D11Buffer** outBuffer)
	{
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = usage;
		bufferDesc.ByteWidth = sizeof( T );
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;

		HRESULT result = device->CreateBuffer( &bufferDesc, &initData, outBuffer );
		return result;
	}
}
