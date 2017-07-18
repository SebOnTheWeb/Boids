#include "StorageBuffer.h"
#include "Renderer.h"

//Constructors and deconstructor
StorageBuffer::StorageBuffer(Renderer* rendererPtr) {
	this->rendererPtr = rendererPtr;
}

StorageBuffer::~StorageBuffer() {

}