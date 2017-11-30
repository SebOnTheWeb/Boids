#pragma once

#include <glm\glm.hpp>

#include "StorageBuffer.h"

struct GridVertex {
	glm::vec3 position;
	glm::vec3 color;
};

class GridCube {
private:
	const glm::vec3 GRID_COLOR = glm::vec3(0.0f, 0.0f, 0.0f);

	float sideLength;
	int gridDensity; //For a lack of better word
	glm::vec3 cubeCenterPos;

	int nrOfGridVertices;
	StorageBuffer* gridDataBufferPtr;

	//Helper functions
	GridVertex* GenerateGridVertices();

public:
	//Constructors and deconstructor
	GridCube();
	GridCube(Renderer* rendererPtr, float sideLength, int gridThickness, glm::vec3 cubeCenterPos);
	~GridCube();

	//Getters
	float GetSideLength() const;
	glm::vec3 GetCubeCenterPos() const;
	int GetNrOfGridVertices() const;
	StorageBuffer* GetGridDataBuffer();
};