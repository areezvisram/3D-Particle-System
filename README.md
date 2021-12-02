## **OpenGL + GLUT 3D Particle System**

![image](https://i.imgur.com/HyC89Ca.png)

### **Program Execution**
Using the makefile, the program can be executed via:
* `all`: Run `make`

To remove the executable run:
* `clean`: Run `make clean`

### **Main Program**
**External Libraries Used**:
* iostream
    * Used for basic input and output
* random
    * Used in the generation of random values within a passed range

#### **Notes:**
**PLEASE NOTE: The bindings are CASE SENSITIVE**

A list of the key bindings and the action they pertain to:

**KEYBOARD COMMANDS (case sensitive)**
* f - Toggle friction mode on and off
* Space bar - Stop and start the simulation (currently animating particles finish animation until they die, but no new particles are created)
* r - Clear all the particles and restart the fountain
* q - Quit Program
* l - Toggle lighting on and off
* w - Move Light 0 negative five units in the X direction
* s - Move Light 0 positive five units in the X direction
* a - Move Light 0 positive five units in the Z direction
* d - Move Light 0 negative five units in the Z direction
* u - Move Light 1 negative five units in the X direction
* j - Move Light 1 positive five units in the X direction
* h - Move Light 1 positive five units in the Z direction
* k - Move Light 1 negative five units in the Z direction
* p - Pause the simulation (simulation is completely paused)
* c - Toggle particle camera mode. Pressing 'c' again while in particle camera mode switches the camera to the particle that most recently came out of the fountain
* C - Exit particle camera mode. Can only be used while in particle camera mode
* t - Toggle particle trails on and off
* v - Toggle the view of light spheres on and off
* UP_ARROW - Rotate entire scene negative 5 units on the X axis
* DOWN_ARROW - Rotate entire scene positive 5 units on the X axis
* RIGHT_ARROW - Rotate entire scene negative 5 units on the Y axis
* LEFT_ARROW - Rotate entire scene positive 5 units on the Y axis

#### **Features:**
**Particle Fountain**
* Particles are generated from a fountain, first being shot upwards and bouncing towards the ground
* Random initial velocity and direction for every particle
* Each particle has a lifespan, dying after a certain, random period of time
* If particles fall off the edge of the floor, they also die

**Particle Cam**
* Clicking the 'c' key brings the user to particle cam
* It is a first person view of the particle that most recently came out of the fountain
* Clicking 'c' again switches the point of view to the now most recent particle
* Clicking 'C' brings the user out of particle cam mode
* Particle cam retains scene rotation
* So if you rotate and then go into particle cam, and then exit, the rotation will be maintained
* When in particle cam, you will automatically be brought back to scene view when the particle you are viewing dies

**Particle Trails**
* Particle trails are off by defualt
* Clicking the 't' key will toggle particle tails on and off
* Particles trails have a fixed age meaning that they will all last for the same duration before being deleted
* When the particle is not moving, no trails are created from it

**Object Collisions**
* 3 solid spheres exist on the plane of the scene
* Particles can bounce off each of the spheres using collision detection
* When friction mode is on, particles lose velocity when bouncing, if off, particles do not lose velocity

#### **Design Decisions**
* When clicking 'r' to reset the simulation, the fountain retains the current start/stop state
* If fountain is on when 'r' is clicked, particles will continue to come out of the fountain after the reset, without needing to click anything else
* If fountain is off when 'r' is clicked, particles will continue to not come out of the fountain after the reset, space will need to be clicked again to restart the simulation
* Adding a command to pause the simulation entirely, using 'p', all particles freeze in their current position and no new particles come out of the fountain
* Light spheres can be viewed by pressing the 'v' key
* This draws two spheres in the current locations of the 2 lights, showing the user where they are at any given time
* **NOTE** This does NOT turn the lights on and off. It simply just shows where the lights currently are.
