#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <random>
#include <particle.h>
#include <particleSystem.h>
#include <mathLib3D.h>
#include <material.h>

// Global variables
float GLOBAL_WIDTH = 800;
float GLOBAL_HEIGHT = 800;
float cameraX = 650;
float cameraY = 200;
float cameraZ = 100;

float cameraDirX = 0;
float cameraDirY = 50;
float cameraDirZ = 0;

bool frictionMode = true;
bool animating = true;
bool simulating = true;
bool lightsOn = true;
bool cameraMode = false;
bool showTrails = false;
bool showLights = false;

float lightPos[2][4] = {
    {-20, 200, -20, 1},
    {20, 200, 20, 1}};

float lightAmb[2][4] = {
    {0.1, 0.1, 0.1, 1},
    {0.1, 0.1, 0.1, 1}};

float lightDiff[2][4] = {
    {1, 1, 1, 1},
    {1, 1, 1, 1}};

float lightSpec[2][4] = {
    {1, 1, 1, 1},
    {1, 1, 1, 1}};

float lightEmis[2][4] = {
    {1, 1, 1, 1},
    {1, 1, 1, 1}};

float materials[4][4][4] = {
    {{0.0f, 0.1f, 0.06f, 1.0f}, //cyan plastic
     {0.0f, 0.50980392f, 0.50980392f, 1.0f},
     {0.50196078f, 0.50196078f, 0.50196078f, 1.0f}},
    {{0.1745f, 0.01175f, 0.01175f, 0.55f}, //ruby
     {0.61424f, 0.04136f, 0.04136f, 0.55f},
     {0.727811f, 0.626959f, 0.626959f, 0.55f}},
    {{0.0215f, 0.1745f, 0.0215f, 0.55f}, //emerald
     {0.07568f, 0.61424f, 0.07568f, 0.55f},
     {0.633f, 0.727811f, 0.633f, 0.55f}},
    {{0.05375f, 0.05f, 0.06625f, 0.82f}, //obsidian
     {0.18275f, 0.17f, 0.22525f, 0.82f},
     {0.332741f, 0.328634f, 0.346435f, 0.82f}},
};

Material mats[4] = {
    Material(materials[0][0], materials[0][1], materials[0][2], 10),
    Material(materials[1][0], materials[1][1], materials[1][2], 10),
    Material(materials[2][0], materials[2][1], materials[2][2], 10),
    Material(materials[3][0], materials[3][1], materials[3][2], 10)};

float sphere_ambient[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float sphere_diffuse[4] = {0.01f, 0.01f, 0.01f, 1.0f};
float sphere_specular[4] = {0.50f, 0.50f, 0.50f, 1.0f};
float sphere_shine = 32.0f;

int sceneRotation[2] = {0, 0}; // [0] about y, [1] about x
int savedSceneRotation[2] = {0, 0};
ParticleSystem particleSystem;
Point3D spheres[3] = {Point3D(100, 30, 50), Point3D(-100, 30, -100), Point3D(-50, 30, 50)};

// Draw individual particle
void drawParticle(Particle particle)
{
    glPushMatrix();
    glTranslatef(particle.pos.x, particle.pos.y, particle.pos.z);
    glRotatef(particle.rot.x, 1, 0, 0);
    glRotatef(particle.rot.y, 0, 1, 0);
    glRotatef(particle.rot.z, 0, 0, 1);
    glColor3f(particle.colour.r, particle.colour.g, particle.colour.b);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mats[particle.matIndex].ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mats[particle.matIndex].diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mats[particle.matIndex].specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mats[particle.matIndex].shiny);
    glutSolidCube(particle.size);
    glPopMatrix();

    // Show particle trails when toggled on
    if (showTrails)
    {
        for (long long unsigned int i = 0; i < particle.trail.size(); i++)
        {
            glPushMatrix();
            Particle tp = particle.trail.at(i);
            glTranslatef(tp.pos.x, tp.pos.y, tp.pos.z);
            glPointSize(2.5);
            glBegin(GL_POINTS);
            glColor3f(tp.colour.r, tp.colour.g, tp.colour.b);
            glVertex3f(0,0,0);
            glEnd();
            glPopMatrix();
        }
    }
}

// Draw sphere - used to draw spheres on plane that particles can collide with
void drawSphere(Point3D center)
{
    glPushMatrix();
        glColor3f(0.1, 0.1, 0.1);
        glTranslatef(center.x, center.y, center.z);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sphere_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sphere_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sphere_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, sphere_shine);
        glutSolidSphere(30, 20, 20);
    glPopMatrix();
}

// Draw the floor on which particles bounce
void drawFloor()
{
    glPushMatrix();
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.3, 1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mats[3].ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mats[3].diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mats[3].specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, mats[3].shiny);
    glNormal3f(0, 1, 0);
    glVertex3f(200, 0, 200);
    glVertex3f(200, 0, -200);
    glVertex3f(-200, 0, -200);
    glVertex3f(-200, 0, 200);
    glEnd();
    glPopMatrix();
}

// Draw all particles currently in the particle system
void drawParticles()
{
    for (long long unsigned int i = 0; i < particleSystem.particles.size(); i++)
    {
        drawParticle(particleSystem.particles.at(i));

        Particle p = particleSystem.particleToFollow;
        if (!(p.pos.x == 0 && p.pos.y == -1 && p.pos.z == 0))
        {
            drawParticle(p);
        }
    }
}

// Toggle particle camera on and off
void toggleParticleCam()
{
    cameraMode = true;
    sceneRotation[0] = 0;
    sceneRotation[1] = 0;
    particleSystem.particleToFollow = particleSystem.particles.back();
    particleSystem.particles.pop_back();
}

// Keyboard function
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
    case 'Q':
        exit(0);
        break;
    case 'f':
    case 'F':
        frictionMode = !frictionMode;
        break;
    case 32:
        animating = !animating;
        break;
    case 'r':
    case 'R':
        particleSystem.particles.clear();
        break;
    case 'l':
    case 'L':
        lightsOn = !lightsOn;
        if (lightsOn)
        {
            glEnable(GL_LIGHTING);
        }
        else
        {
            glDisable(GL_LIGHTING);
        }
        break;
    case 'w':
        lightPos[0][0] -= 5;
        break;
    case 's':
        lightPos[0][0] += 5;
        break;
    case 'a':
        lightPos[0][2] += 5;
        break;
    case 'd':
        lightPos[0][2] -= 5;
        break;
    case 'u':
        lightPos[1][0] -= 5;
        break;
    case 'j':
        lightPos[1][0] += 5;
        break;
    case 'h':
        lightPos[1][2] += 5;
        break;
    case 'k':
        lightPos[1][2] -= 5;
        break;
    case 'p':
        simulating = !simulating;
        break;
    case 'c':
        if (particleSystem.particles.size() > 0)
            toggleParticleCam();
        break;
    case 'C':
        if(cameraMode) {
            cameraMode = !cameraMode;
            cameraX = 650;
            cameraY = 200;
            cameraZ = 100;

            cameraDirX = 0;
            cameraDirY = 50;
            cameraDirZ = 0;

            sceneRotation[0] = savedSceneRotation[0];
            sceneRotation[1] = savedSceneRotation[1];
        }
        break;
    case 't':
        showTrails = !showTrails;
        break;
    case 'v':
        showLights = !showLights;
        break;
    }
}

// Special keyboard function
void special(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_RIGHT:
        sceneRotation[0] -= 5;
        sceneRotation[0] = sceneRotation[0] % 360;
        savedSceneRotation[0] = sceneRotation[0];
        break;
    case GLUT_KEY_LEFT:
        sceneRotation[0] += 5;
        sceneRotation[0] = sceneRotation[0] % 360;
        savedSceneRotation[0] = sceneRotation[0];
        break;
    case GLUT_KEY_UP:
        sceneRotation[1] -= 5;
        sceneRotation[1] = sceneRotation[1] % 360;
        savedSceneRotation[1] = sceneRotation[1];
        break;
    case GLUT_KEY_DOWN:
        sceneRotation[1] += 5;
        sceneRotation[1] = sceneRotation[1] % 360;
        savedSceneRotation[1] = sceneRotation[1];
        break;
    }
}

// Display function
void display(void)
{
    float clearEmis[4] = {0, 0, 0, 1};
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ, cameraDirX, cameraDirY, cameraDirZ, 0, 1, 0);

    glPushMatrix();
    glRotatef(sceneRotation[0], 0, 1, 0);
    glRotatef(sceneRotation[1], 1, 0, 0);
    drawFloor();
    drawSphere(spheres[0]);
    drawSphere(spheres[1]);
    drawSphere(spheres[2]);
    drawParticles();

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos[0]);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb[0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff[0]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec[0]);
    glColor3f(1, 1, 1);

    if (showLights)
    {
        glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, lightEmis[0]);
        glTranslatef(lightPos[0][0], lightPos[0][1], lightPos[0][2]);
        glutSolidSphere(5, 16, 16);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, clearEmis);
        glPopMatrix();
    }

    glLightfv(GL_LIGHT1, GL_POSITION, lightPos[1]);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb[1]);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiff[1]);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpec[1]);
    glColor3f(1, 1, 1);

    if (showLights)
    {
        glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, lightEmis[1]);
        glTranslatef(lightPos[1][0], lightPos[1][1], lightPos[1][2]);
        glutSolidSphere(5, 16, 16);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, clearEmis);
        glPopMatrix();
    }

    glPopMatrix();

    glutSwapBuffers();
}

// Timer function
void timer(int value)
{
    if (simulating)
    {

        if (animating && value % 2 == 0)
        {
            particleSystem.generateParticle();
        }

        particleSystem.update(frictionMode, spheres, value);

        if (cameraMode)
        {
            Particle particle = particleSystem.particleToFollow;
            if (particle.age <= 0)
            {
                cameraMode = false;
                particleSystem.particleToFollow = Particle(Point3D(0, -1, 0));

                sceneRotation[0] = savedSceneRotation[0];
                sceneRotation[1] = savedSceneRotation[1];
                cameraX = 650;
                cameraY = 200;
                cameraZ = 100;

                cameraDirX = 0;
                cameraDirY = 50;
                cameraDirZ = 0;
            }
            else
            {
                cameraX = particle.pos.x;
                cameraY = particle.pos.y;
                cameraZ = particle.pos.z;

                cameraDirX = 0;
                cameraDirY = 0;
                cameraDirZ = 0;
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(17, timer, value + 1);
}

// Reshape function
void reshape(int w, int h)
{
    GLOBAL_WIDTH = w;
    GLOBAL_HEIGHT = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

// Initialize glut 
void initGlut()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("3D Particle System");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(17, timer, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
}

// Print all keyboard commands
void printCommands () {
    std::cout << "KEYBOARD COMMANDS (case sensitive)" << "\n";
    std::cout << "a - Add new particle at mouse position" << "\n";
    std::cout << "d - Delete particle nearest to mouse position" << "\n";
    std::cout << "+ - Increase global range that particles can attract/repel within" << "\n";
    std::cout << "- - Increase global range that particles can attract/repel within" << "\n";
    std::cout << "UP ARROW - Increase global speed that particles move with" << "\n";
    std::cout << "DOWN ARROW - Decrease global speed that particles move with" << "\n";
    std::cout << "Space bar - Pause the simulation" << "\n";
    std::cout << "r - Reset animation with brand new particles" << "\n";
    std::cout << "q - Quit Program" << "\n";
    std::cout << "ESC - Quit Program" << "\n";
    std::cout << "c - Toggle display of range of each individual particle" << "\n";
    std::cout << "C - Toggle display of global range of mouse cursor" << "\n";
    std::cout << "b - Arrange the particles in a circle shape" << "\n";
    std::cout << "z - Arrange the particles in a flower shape" << "\n";
    std::cout << "v - Toggle display of direction vectors of each particle" << "\n";

    std::cout << "f - Toggle friction mode on and off" << "\n";
    std::cout << "Space bar - Stop and start the simulation (currently animating particles finish animation until they die, but no new particles are created)" << "\n";
    std::cout << "r - Clear all the particles and restart the fountain" << "\n";
    std::cout << "q - Quit Program" << "\n";
    std::cout << "l - Toggle lighting on and off" << "\n";
    std::cout << "w - Move Light 0 negative five units in the X direction" << "\n";
    std::cout << "s - Move Light 0 positive five units in the X direction" << "\n";
    std::cout << "a - Move Light 0 positive five units in the Z direction" << "\n";
    std::cout << "d - Move Light 0 negative five units in the Z direction" << "\n";
    std::cout << "u - Move Light 1 negative five units in the X direction" << "\n";
    std::cout << "j - Move Light 1 positive five units in the X direction" << "\n";
    std::cout << "h - Move Light 1 positive five units in the Z direction" << "\n";
    std::cout << "k - Move Light 1 negative five units in the Z direction" << "\n";
    std::cout << "p - Pause the simulation (simulation is completely paused)" << "\n";
    std::cout << "c - Toggle particle camera mode. Pressing 'c' again while in particle camera mode switches the camera to the particle that most recently came out of the fountain" << "\n";
    std::cout << "C - Exit particle camera mode. Can only be used while in particle camera mode" << "\n";
    std::cout << "t - Toggle particle trails on and off" << "\n";
    std::cout << "v - Toggle the view of light spheres on and off" << "\n";
    std::cout << "UP_ARROW - Rotate entire scene negative 5 units on the X axis" << "\n";
    std::cout << "DOWN_ARROW - Rotate entire scene positive 5 units on the X axis" << "\n";
    std::cout << "RIGHT_ARROW - Rotate entire scene negative 5 units on the Y axis" << "\n";
    std::cout << "LEFT_ARROW - Rotate entire scene positive 5 units on the Y axis" << "\n";
}

// Initialize program
void init()
{
    printCommands();
    initGlut();
    glClearColor(0.6, 0.6, 0.6, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    particleSystem = ParticleSystem(Point3D(0, 10, 0));
}

// main function - entry of the program
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    init();
    glutMainLoop();
    return (0);
}
