#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include "mathLib3D.h"
#include "colour.h"

class Particle{
    public:
        Particle();
        Particle(Point3D position);
        Particle(Point3D position, Vec3D direction, Colour colour, int matIndex, float speed, int size, Vec3D rotationAngle, int material, int age);
        Point3D pos;
        Colour colour;
        int matIndex;
        Vec3D dir;
        float speed;
        int size;
        Vec3D rot;
        Vec3D rotInc;
        int mat;
        int age;
        std::vector<Particle> trail;

        void update(Vec3D gravity, bool friction, Point3D spheres[3], int timerVal);
};

#endif 