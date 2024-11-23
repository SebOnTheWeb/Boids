#pragma once

const float			MIN_SEPERATION_DISTANCE = 2.0f;
const unsigned int	COHESION_THRESHHOLD = 2;
const float			BOID_SPEED = 1.0f;
const float			MAX_SPEED = 8.0f;
const float			MIN_SPEED = 0.5f;
const float			MAX_ACCELERATION = 1.0f;

//Rule factors
const float			CENTER_FACTOR = 0.02f;
const float			AVOID_FACTOR = 1.0f;
const float			MATCH_FACTOR = 0.125;

/* VALID FLOCK SIZES
64
512
4096
*/
const unsigned int NR_OF_BOIDS = 64; //Must be dividable by 64
const float BOID_SEPERATION = 3.0f; //Init position seperation

const float GRID_SIDE_LENGTH = 40.0f;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 800;
const int NR_OF_SEC_TO_MEASURE = 60;

enum LogicUpdateType {
	SINGLE_CPU = 0,
	MULTI_CPU = 1,
	GPU = 2
};

const LogicUpdateType LOGIC_UPDATE_TYPE = GPU;

/*
This is a bit of a fast and ugly way of not having to use "constants." infront of every constant
while at the same time having a struct for sending const data into GPU.
*/
struct Constants {
	float			GPU_MIN_SEPERATION_DISTANCE = MIN_SEPERATION_DISTANCE;
	unsigned int	GPU_COHESION_THRESHHOLD = COHESION_THRESHHOLD;
	float			GPU_BOID_SPEED = BOID_SPEED;
	float			GPU_MAX_SPEED = MAX_SPEED;
	float			GPU_MIN_SPEED = MIN_SPEED;
	float			GPU_MAX_ACCELERATION = MAX_ACCELERATION;

	float			GPU_CENTER_FACTOR = CENTER_FACTOR;
	float			GPU_AVOID_FACTOR = AVOID_FACTOR;
	float			GPU_MATCH_FACTOR = MATCH_FACTOR;

	unsigned int	GPU_NR_OF_BOIDS = NR_OF_BOIDS;
	float			GPU_BOID_SEPERATION = BOID_SEPERATION;

	float			GPU_GRID_SIDE_LENGTH = GRID_SIDE_LENGTH;

}; static Constants constants;