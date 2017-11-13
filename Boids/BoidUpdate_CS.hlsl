struct Boid {
	float3 position: POSITION;
	float3 velocity: VELOCITY;
	float3 up: UP;
};

struct Constants
{
    float MIN_SEPERATION_DISTANCE;
    uint COHESION_THRESHHOLD;
    float BOID_SPEED;
    float MAX_SPEED;
    float MIN_SPEED;
    float MAX_ACCELERATION;

    float CENTER_FACTOR;
    float AVOID_FACTOR;
    float MATCH_FACTOR;

    uint NR_OF_BOIDS;
    float BOID_SEPERATION;

    float GRID_SIDE_LENGTH;
};

StructuredBuffer<Boid> readBufferBoids : register(t0);
RWStructuredBuffer<Boid> writeBufferBoids : register(u0);

StructuredBuffer<float> readBufferDeltaTime : register(t1);

StructuredBuffer<Constants> readBufferConstants : register(t2);


float3 CenterRule(int currentBoidIndex) {
    float3 center = 0.0f;

    for (int i = 0; i < readBufferConstants[0].NR_OF_BOIDS; i++) {
        center += readBufferBoids[i].position;
    }
    center -= readBufferBoids[currentBoidIndex].position;
    center = center / (float) (readBufferConstants[0].NR_OF_BOIDS - 1);

    return center * readBufferConstants[0].CENTER_FACTOR;
}

float3 AvoidRule(int currentBoidIndex) {
    float3 avoid = 0.0f;
    float3 currentBoidPos = readBufferBoids[currentBoidIndex].position;
    float3 vecToBoid = 0.0f;

    for (int i = 0; i < readBufferConstants[0].NR_OF_BOIDS; i++) {
        if (i != currentBoidIndex) {
            vecToBoid = readBufferBoids[i].position - readBufferBoids[currentBoidIndex].position;
            if (length(vecToBoid) < readBufferConstants[0].MIN_SEPERATION_DISTANCE)
            {
                avoid -= vecToBoid;
            }
        }
    }

    return avoid * readBufferConstants[0].AVOID_FACTOR;
}

float3 VelocityRule(int currentBoidIndex) {
    float3 velocity = 0.0f;
    
    for (int i = 0; i < readBufferConstants[0].NR_OF_BOIDS; i++) {
        velocity += readBufferBoids[i].velocity;
    }
    velocity -= readBufferBoids[currentBoidIndex].velocity;
    velocity = velocity / (float) (readBufferConstants[0].NR_OF_BOIDS - 1);

    return velocity * readBufferConstants[0].MATCH_FACTOR;
}

float3 LimitSpeed(float3 oldVelocity, float3 newVelocity) {
    float3 limitedVelocity = newVelocity;
    float newSpeed = length(newVelocity);
    float oldSpeed = length(oldVelocity);

    if (newSpeed > readBufferConstants[0].MAX_SPEED || newSpeed < readBufferConstants[0].MIN_SPEED)
    {
        limitedVelocity = oldVelocity;
    }
    else {
        if (newSpeed > oldSpeed) {
            limitedVelocity = normalize(limitedVelocity) * (oldSpeed + readBufferConstants[0].MAX_ACCELERATION);
        }
        else {
            limitedVelocity = normalize(limitedVelocity) * (oldSpeed + -readBufferConstants[0].MAX_ACCELERATION);
        }
    }

    return limitedVelocity;
}

float3 CalculateNewUp(float3 newVelocity) {
    float3 forward = normalize(newVelocity);
    float3 newRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), forward));
    float3 newUp = cross(forward, newRight);

    return newUp;
}

float3 CalculateNewPos(float3 oldPosition, float3 newVelocity) {
    float deltaTime = readBufferDeltaTime[0];
    float3 newPos = oldPosition + (newVelocity * deltaTime * readBufferConstants[0].BOID_SPEED);

    //TODO: Handle grid cube limitations;

    return newPos;
}

float3 MoveIfOutOfBounds(float3 position) {
    float3 newPosition = position;

    float sideLength = readBufferConstants[0].GRID_SIDE_LENGTH;

    float xMax = 0.0f + (sideLength / (float) 2);
    float xMin = 0.0f - (sideLength / (float) 2);
    float yMax = 0.0f + (sideLength / (float) 2);
    float yMin = 0.0f - (sideLength / (float) 2);
    float zMax = 0.0f + (sideLength / (float) 2);
    float zMin = 0.0f - (sideLength / (float) 2);

    //X
    if (position.x > xMax) {
        newPosition.x = xMin;
    }
    if (position.x < xMin) {
        newPosition.x = xMax;
    }
	//Y
    if (position.y > yMax) {
        newPosition.y = yMin;
    }
    if (position.y < yMin) {
        newPosition.y = yMax;
    }
	//Z
    if (position.z > zMax) {
        newPosition.z = zMin;
    }
    if (position.z < zMin) {
        newPosition.z = zMax;
    }

    return newPosition;
}

[numthreads(1, 1, 1)] //TODO: 64 numthreads?
void main( uint3 DTid : SV_DispatchThreadID ) {
    int i = DTid.x;
    float3 previousVelocty = readBufferBoids[i].velocity;
    float3 newVelocity = previousVelocty;

	//1. Fly towards center
    float3 centerRuleVec = CenterRule(i);

	//2. Avoid boids
    float3 avoidRuleVec = AvoidRule(i);

	//3. Match velocity/direction with all boids
    float3 velocityRuleVec = VelocityRule(i);

	//Add all rules
    newVelocity += centerRuleVec + avoidRuleVec + velocityRuleVec;

	//Limit speed
    newVelocity = LimitSpeed(previousVelocty, newVelocity);

	//Set new boid velocity
    writeBufferBoids[i].velocity = newVelocity;
    
    //Set new boid up
    writeBufferBoids[i].up = CalculateNewUp(newVelocity);

	//Calculate new boid position
    float3 oldPosition = readBufferBoids[i].position;
    float3 newPosition = CalculateNewPos(oldPosition, newVelocity);

    //Move if out of bounds
    newPosition = MoveIfOutOfBounds(newPosition);

    //Set boid new position
    writeBufferBoids[i].position = newPosition;
}