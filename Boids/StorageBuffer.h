#pragma once

class Renderer;

class StorageBuffer {
private:
	Renderer* rendererPtr;

public:
	//Construtor and deonstructor
	StorageBuffer();
	StorageBuffer(Renderer* rendererPtr);
	~StorageBuffer();
};