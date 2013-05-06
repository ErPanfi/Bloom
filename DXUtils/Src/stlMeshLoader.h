//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <d3d11.h>

#include "types.h"

namespace mcg
{
	class STLMeshLoader
	{
	public:
		STLMeshLoader();

		bool loadFormFile(const std::string& fileName);

		void swapHandness();
		void smoothNormals(const float degAngleThreshold);
		
		bool getVertexBuffer(ID3D11Device* device, ID3D11Buffer** vertexBuffer);
		bool getIndexBuffer(ID3D11Device* device, ID3D11Buffer** indexBuffer);

		unsigned int getNumberOfVertices() const;
		unsigned int getNumberOfTriangles() const;
		unsigned int getNumberOfIndices() const;

	private:
		bool reeadFromFile(std::ifstream& file);
		bool readHeader(std::ifstream& file);
		bool readMesh(std::ifstream& file);
		bool readTriangle(std::ifstream& file);

		template<typename T>
		T readData(std::ifstream& file);
		std::vector<float> computeFacesArea() const;

		std::string mMeshName;
		std::vector<mcg::PositionNormal> mMesh;
		std::vector<unsigned int> mIndices;
	};

	template<typename T>
	T STLMeshLoader::readData(std::ifstream& file)
	{
		T data;
		char* ptr = reinterpret_cast<char*>(&data);
		file.read(ptr, sizeof(T));
		return data;
	}
}
