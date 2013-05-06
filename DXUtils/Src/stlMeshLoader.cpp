//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "stlMeshLoader.h"
#include "buffer.h"

#include <DirectXMath.h>
#include <algorithm>
#include <map>
#include <set>

using namespace mcg;
using namespace DirectX;

union BinFileData
{
	char cData[4];
	unsigned int iData;
	float fData;
};

STLMeshLoader::STLMeshLoader()
	: mMeshName("")
{}

bool STLMeshLoader::loadFormFile(const std::string& fileName)
{
	std::ifstream file(fileName.c_str(), std::ios::binary);
	if(!file.is_open())
		return false;

	return reeadFromFile(file);
}

bool STLMeshLoader::reeadFromFile(std::ifstream& file)
{
	if(!readHeader(file))
		return false;

	if(!readMesh(file))
		return false;

	return true;
}

bool STLMeshLoader::readHeader(std::ifstream& file)
{
	char header[81];
	file.read(header, 80);
	header[80] = 0;

	mMeshName = header;

	return file.good();
}

bool STLMeshLoader::readMesh(std::ifstream& file)
{
	unsigned int numberOfTriangles = 0;
	numberOfTriangles = readData<unsigned int>(file);

	mMesh.reserve(numberOfTriangles * 3);
	mIndices.reserve(numberOfTriangles * 3);

	for(unsigned int i = 0; i < numberOfTriangles; ++i)
	{
		if(!readTriangle(file))
			break;

		mIndices.push_back(i * 3);
		mIndices.push_back(i * 3 + 1);
		mIndices.push_back(i * 3 + 2);
	}

	return file.good();
}

bool STLMeshLoader::readTriangle(std::ifstream& file)
{
	XMFLOAT3 normal = readData<XMFLOAT3>(file);

	for(int i = 0; i < 3; ++i)
	{
		PositionNormal vertex;
		vertex.normal = normal;
		vertex.position = readData<XMFLOAT3>(file);

		mMesh.push_back(vertex);
	}
	unsigned short flags = readData<unsigned short>(file);

	return file.good();
}

bool STLMeshLoader::getVertexBuffer(ID3D11Device* device, ID3D11Buffer** vertexBuffer)
{
	HRESULT result = createVertexBuffer(device, mMesh, D3D11_USAGE_DEFAULT, vertexBuffer);
	return !FAILED(result);
}

bool STLMeshLoader::getIndexBuffer(ID3D11Device* device, ID3D11Buffer** indexBuffer)
{
	HRESULT result = createIndexBuffer(device, mIndices, D3D11_USAGE_DEFAULT, indexBuffer);
	return !FAILED(result);
}

unsigned int STLMeshLoader::getNumberOfVertices() const
{
	return mMesh.size();
}

unsigned int STLMeshLoader::getNumberOfTriangles() const
{
	return getNumberOfIndices() / 3;
}

unsigned int STLMeshLoader::getNumberOfIndices() const
{
	return mIndices.size();
}

void STLMeshLoader::swapHandness()
{
	std::for_each(mMesh.begin(), mMesh.end(),
		[](PositionNormal& vertex)
		{
			vertex.normal.z *= -1.0f;
			vertex.position.z *= -1.0f;
		}
	);

	for(unsigned int i = 0; i < mIndices.size(); i = i+3)
	{
		std::swap(mIndices[i], mIndices[i+1]);
	}
}

bool operator ==(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool operator <(const XMFLOAT3& a, const XMFLOAT3& b)
{
	if(a == b)
		return false;

	if(a.x < b.x)
		return true;
	else if(a.x == b.x)
	{
		if(a.y < b.y)
			return true;
		else if(a.y == b.y && a.z < b.z)
			return true;
		else
			return false;
	}
	else
		return false;
}

class Comp
{
public:
	bool operator()(const XMFLOAT3& a, const XMFLOAT3& b) const
	{
		return a < b;
	}
};

void STLMeshLoader::smoothNormals(const float degAngleThreshold)
{
	float angleThreshold = std::cos(XMConvertToRadians(degAngleThreshold));
	std::vector<float> areas = computeFacesArea();
	std::map<XMFLOAT3, std::vector<std::pair<XMFLOAT3, int>>, Comp> vertexMap;

	for(unsigned int i = 0; i < mMesh.size(); ++i)
	{
		PositionNormal& vert = mMesh[i];
		vertexMap[vert.position].push_back(std::make_pair(vert.normal, i));
	}

	for(unsigned int i = 0; i < mMesh.size(); ++i)
	{
		PositionNormal& vert = mMesh[i];
		XMVECTOR normal = XMLoadFloat3(&vert.normal);
		XMVECTOR newNormal = XMVectorZero();

		std::vector<std::pair<XMFLOAT3, int>>& normals = vertexMap[vert.position];
		for(auto nIt = normals.begin(); nIt != normals.end(); ++nIt)
		{
			XMVECTOR otherNormal = XMLoadFloat3(&(nIt->first));
			XMFLOAT3 dotRes;
			XMStoreFloat3(&dotRes, XMVector3Dot(normal, otherNormal));

			if(dotRes.x > angleThreshold)
			{
				int triangleIndex = static_cast<int>( std::floor(nIt->second / 3.0f) );
				newNormal = XMVectorAdd(newNormal, XMVectorScale(otherNormal, areas[triangleIndex]));
				newNormal = XMVectorAdd(newNormal, otherNormal);
			}
		}

		newNormal = XMVector3Normalize(newNormal);
		XMStoreFloat3(&vert.normal, newNormal);
	}
}

std::vector<float> STLMeshLoader::computeFacesArea() const
{
	std::vector<float> areas;
	areas.reserve(getNumberOfTriangles());

	for(unsigned int i = 0; i < mIndices.size(); i = i+3)
	{
		XMVECTOR v0 = XMLoadFloat3(&mMesh[mIndices[i]].position);
		XMVECTOR v1 = XMLoadFloat3(&mMesh[mIndices[i+1]].position);
		XMVECTOR v2 = XMLoadFloat3(&mMesh[mIndices[i+2]].position);

		XMVECTOR v01 = XMVectorSubtract(v1, v0);
		XMVECTOR v02 = XMVectorSubtract(v2, v0);

		XMVECTOR crossLen = XMVector3Length(XMVector3Cross(v01, v02));
		XMFLOAT3 length;
		XMStoreFloat3(&length, crossLen);

		areas.push_back(length.x / 2.0f);
	}

	return areas;
}
