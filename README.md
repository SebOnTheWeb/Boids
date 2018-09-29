
# Boids on CPU and GPU
## Introduction
My implementation of boids for my bachelor thesis at Blekinge Institute of Technology. The thesis was based on measuring and comparing a boid simulation running on singe-core CPU, multi-core CPU and GPU.
The project was built in Visual studio 2017. Rendering was done with DirectX 11 and glm was used for vector fucntionality. \
I have since refactored the project on my free time to improve my skills in refactoring and clean code.


![[Initial positions image]](images/Flock64Cube40.jpg?raw=true "Flocksize 64 and grid size 40, initial positions")

![[Simulation image]](images/Flock512Cube40Active.jpg?raw=true "Flocksize 512 and grid size 40, active simulation")
## Controls
Start/pause boid logic by pressing SPACE \
Move the camera with WASD \
Rotate camera with QE \
Change camera direction by moving the mouse while pressing and holding RMB \
Increase camera movement speed by pressing and holding LSHIFT \
Exit with ESC

## Settings
Boid and grid properties can be set in Constants.h \
Computation settings can be set in main.cpp

## Implementation
- This boid algorithm follows the three basic rules; alignment, velocity matching and centering.
- Boids have a maximum and minimum speed.
- Boids start facing in a random direction.
- Boids leaving the grid will be moved to the opposite side.
- Upon start boid logic is paused and can be started with SPACE.
- Camera can be moved while logic is paused.
