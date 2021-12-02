#include <particleSystem.h>
#include <random.h>
#include <particle.h>
#include <colour.h>
#include <iostream>

const Vec3D ParticleSystem::gravity = Vec3D(0,-0.2,0);

ParticleSystem::ParticleSystem() {
    origin = Point3D();
    particleToFollow = Particle(Point3D(0,-1,0));
}

ParticleSystem::ParticleSystem(Point3D origin) {
    this->origin = origin;
    particleToFollow = Particle(Point3D(0,-1,0));
}

void ParticleSystem::generateParticle()
{    
    Particle p = Particle(
        origin,
        Vec3D(Random::generateRandomFloat(-1, 1),Random::generateRandomFloat(15, 25),Random::generateRandomFloat(-1, 1)).normalize(),
        Colour(Random::generateRandomFloat(0, 1),Random::generateRandomFloat(0, 1),Random::generateRandomFloat(0, 1)),
        (int)Random::generateRandomFloat(0, 2.90),
        Random::generateRandomFloat(8, 10),
        (int)Random::generateRandomFloat(3, 7),
        Vec3D(Random::generateRandomFloat(2, 10),Random::generateRandomFloat(2, 10),Random::generateRandomFloat(2, 10)),
        (int)Random::generateRandomFloat(1, 3),
        (int)Random::generateRandomFloat(500, 1000)
    );
    particles.push_back(p);
}

void ParticleSystem::update(bool friction, Point3D spheres[3], int timerVal)
{    
    for(long long unsigned int i = particles.size(); i-- != 0;) {        
        if(particles.empty()) break;        
        if(particles.at(i).age <= 0 || particles.at(i).pos.y < -100) {
            particles.erase(particles.begin() + i);
            break;
        }
        particles.at(i).update(gravity, friction, spheres, timerVal);
    }
    particleToFollow.update(gravity, friction, spheres, timerVal);
}