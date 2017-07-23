#include "StorageBuffer.h"
#include "Renderer.h"

//Constructors and deconstructor
StorageBuffer::StorageBuffer() {
	this->rendererPtr = nullptr;
}

StorageBuffer::StorageBuffer(Renderer* rendererPtr) {
	this->rendererPtr = rendererPtr;
}

StorageBuffer::~StorageBuffer() {

}