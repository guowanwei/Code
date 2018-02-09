#include "stdafx.h"
#include "Terrain.h"
#include <iostream>
#include <fstream>
Terrain::Terrain(WorldTransform transform)
	:Object(transform)
{
	Terrain::InitInfo tii;
	mInfo.HeightMapFilename = L"Textures/terrain.raw";
	mInfo.LayerMapFilename0 = L"Textures/grass.dds";
	mInfo.LayerMapFilename1 = L"Textures/darkdirt.dds";
	mInfo.LayerMapFilename2 = L"Textures/stone.dds";
	mInfo.LayerMapFilename3 = L"Textures/lightdirt.dds";
	mInfo.LayerMapFilename4 = L"Textures/snow.dds";
	mInfo.BlendMapFilename = L"Textures/blend.dds";
	mInfo.HeightScale = 50.0f;
	mInfo.HeightmapWidth = 2049;
	mInfo.HeightmapHeight = 2049;
	mInfo.CellSpacing = 0.5f;
}
bool Terrain::init()
{
	LoadHeightmap();
	return true;
}
void Terrain::update(float Delta)
{

}
void Terrain::render()
{

}
Terrain::~Terrain()
{

}

void Terrain::LoadHeightmap()
{
	// A height for each vertex
	std::vector<unsigned char> in(mInfo.HeightmapWidth * mInfo.HeightmapHeight);

	// Open the file.
	std::ifstream inFile;
	inFile.open(mInfo.HeightMapFilename.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array and scale it.
	mHeightmap.resize(mInfo.HeightmapHeight * mInfo.HeightmapWidth, 0);
	for (UINT i = 0; i < mInfo.HeightmapHeight * mInfo.HeightmapWidth; ++i)
	{
		mHeightmap[i] = (in[i] / 255.0f)*mInfo.HeightScale;
	}
}