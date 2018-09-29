#include "GridCube.h"

//Helper functions
GridVertex* GridCube::GenerateGridVertices() {
	GridVertex* vertices = new GridVertex[nrOfGridVertices];

	int index = 0;

	float posX = 0.0f;
	float posY = 0.0f;
	float posZ = 0.0f;

	int startOffsetX = this->cubeCenterPos.x - (int)(this->sideLength / 2);
	int startOffsetY = this->cubeCenterPos.y - (int)(this->sideLength / 2);
	int startOffsetZ = this->cubeCenterPos.z - (int)(this->sideLength / 2);
	
	float squareSize = this->sideLength / (float)this->gridDensity;

	//########## Top and bottom grid ##########
	for (int j = -1; j <= 1; j += 2) {
		posY = this->cubeCenterPos.y + ((int)(this->sideLength / (float)2) * (float)j);
		for (int i = 0; i < this->gridDensity + 1; i++) {
			//Line parallell with x-axis
			//Point 1
			//Left
			posX = (float)startOffsetX;
			posZ = (float)startOffsetZ + i * squareSize;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;

			//Point 2
			//Right
			posX = (float)startOffsetX + this->sideLength;
			posZ = (float)startOffsetZ + i * squareSize;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;

			//Line parallell with z-axis
			//Point 3
			//Bottom
			posX = (float)startOffsetX + i * squareSize;
			posZ = (float)startOffsetZ;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;

			//Point 4
			//Top
			posX = (float)startOffsetX + i * squareSize;
			posZ = (float)startOffsetZ + this->sideLength;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;
		}
	}

	//########## Left and right grid ##########
	for (int j = -1; j <= 1; j += 2) {
		posX = this->cubeCenterPos.x + ((int)(this->sideLength / (float)2) * (float)j);
		for (int i = 0; i < this->gridDensity + 1; i++) {
			//Line parallell with z-axis
			//Point 1
			//Left
			posZ = (float)startOffsetZ;
			posY = (float)startOffsetY + i * squareSize;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;

			//Point 2
			//Right
			posZ = (float)startOffsetZ + this->sideLength;
			posY = (float)startOffsetY + i * squareSize;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;

			//Line parallell with y-axis
			//Point 3
			//Bottom
			posZ = (float)startOffsetZ + i * squareSize;
			posY = (float)startOffsetY;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;

			//Point 4
			//Top
			posZ = (float)startOffsetZ + i * squareSize;
			posY = (float)startOffsetY + this->sideLength;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;
		}
	}

	//########## Back and front grid ##########
	for (int j = -1; j <= 1; j += 2) {
		posZ = this->cubeCenterPos.z + ((this->sideLength / (float)2) * (float)j);
		for (int i = 0; i < this->gridDensity + 1; i++) {
			//Line parallell with z-axis
			//Point 1
			//Left
			posX = (float)startOffsetX;
			posY = (float)startOffsetY + i * squareSize;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;

			//Point 2
			//Right
			posX = (float)startOffsetX + this->sideLength;
			posY = (float)startOffsetY + i * squareSize;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;

			//Line parallell with y-axis
			//Point 3
			//Bottom
			posX = (float)startOffsetX + i * squareSize;
			posY = (float)startOffsetY;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;

			//Point 4
			//Top
			posX = (float)startOffsetX + i * squareSize;
			posY = (float)startOffsetY + this->sideLength;

			vertices[index].position = glm::vec3(posX, posY, posZ);
			vertices[index].color = glm::vec3(GRID_COLOR);
			index++;
		}
	}

	return vertices;
}

//Constructors and deconstructor
GridCube::GridCube() {
	this->sideLength = 100.0f;
	this->gridDensity = 4;
	this->cubeCenterPos = glm::vec3(0.0f, 0.0f, 0.0f);

	this->nrOfGridVertices = 0;
	this->gridDataBufferPtr = nullptr;
}

GridCube::GridCube(Renderer* rendererPtr, float sideLength, int gridThickness, glm::vec3 cubeCenterPos) {
	this->sideLength = sideLength;
	this->gridDensity = gridThickness;
	this->cubeCenterPos = cubeCenterPos;

	this->nrOfGridVertices = (int)(((this->sideLength + 1) + (this->sideLength + 1)) * 2 * 6);

	GridVertex* gridVertices = GenerateGridVertices();
	this->gridDataBufferPtr = new StorageBuffer(rendererPtr, this->nrOfGridVertices, sizeof(GridVertex));
	this->gridDataBufferPtr->SetData(gridVertices, sizeof(GridVertex) * this->nrOfGridVertices);
	delete gridVertices;
}

GridCube::~GridCube() {
	delete this->gridDataBufferPtr;
	this->gridDataBufferPtr = nullptr;
}

//Getters
float GridCube::GetSideLength() const {
	return this->sideLength;
}

glm::vec3 GridCube::GetCubeCenterPos() const {
	return this->cubeCenterPos;
}

StorageBuffer* GridCube::GetGridDataBuffer() {
	return this->gridDataBufferPtr;
}

int GridCube::GetNrOfGridVertices() const {
	return this->nrOfGridVertices;
}