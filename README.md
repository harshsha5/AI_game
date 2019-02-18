# AI_game

Setup
This software can be run on any OS but I will only detail installation for Ubuntu 14.04. Install cmake, build essentials, X11 libraries, and opengl using the commands sudo apt-get install cmake build-essential xorg-dev libgl1-mesa-dev mesa-utils. 
In order to build the program it requires C++11 compliant compiler, if you are using Ubuntu 14.04 you should have at least gcc 4.84 but you can check using gcc -v. 
Then, within the build directory in /Task4P1 , run “make”. Your executable should now be built and you can run it with ./missiledefense.

AI Overview

The AI works in three simple steps. It first predicts where and when each projectile will land. So basically the program iterates over the list of projectiles to find the predicted time to land to the ground and the time when it will land. This prediction is done by using high school physics- Newton's equations of motion. Using the present position and the velocity of the projectile, we predict the time required for the projectile to land on the ground. The quadratic equation resulted would have two roots- we consider the positive one since time cannot be negative. Prediction pertaining to each of the projectiles are stored in a vector.
This vector is then passed to the determineSafeSpots function which uses this to update a vector counter. Counter vector updates values of each spot based on the time left for collision  and also takes into account the explosion time. This is then used to find what all points on the screen are dangerous and which are not. The danger spots are allowed value 1 and the safe spots are allotted value 0. Then we pick the closest safe spot and move there to ensure the player remains safe. The counter vector is then updated with each passing time step.

Assumptions: The screen is assumed to be discretised into integers from 0 to width of the game screen. 


Author: HARSH SHARMA (harshsha@andrew.cmu.edu)
