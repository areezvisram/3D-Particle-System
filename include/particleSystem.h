#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>
#include "mathLib3D.h"
#include "particle.h"

class ParticleSystem {
    public: 
        ParticleSystem();
        ParticleSystem(Point3D origin);
        Point3D origin;
        std::vector<Particle> particles;
        Particle particleToFollow;

        static const Vec3D gravity;

        void generateParticle();

        void update(bool friction, Point3D spheres[3], int timerVal);
};

#endif