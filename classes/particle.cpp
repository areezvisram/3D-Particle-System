#include <particle.h>
#include <iostream>

Particle::Particle() {
    pos = Point3D();
    colour = Colour();
    matIndex = 0;
    dir = Vec3D();
    speed = 0.0;
    size = 0;
    rot = Vec3D();
    mat = 1;
    age = 0;
}

Particle::Particle(Point3D position) {
    pos = position;
    colour = Colour();
    matIndex = 0;
    dir = Vec3D();
    speed = 0.0;
    size = 0;
    rot = Vec3D();
    mat = 1;
    age = 0;
}

Particle::Particle(Point3D position, Vec3D direction, Colour colour, int matIndex, float speed, int size, Vec3D rotationAngle, int material, int age)
{
    this->pos = position;
    this->dir = direction;
    this->colour = colour;
    this->matIndex = matIndex;
    this->speed = speed;
    this->size = size;
    this->rotInc = rotationAngle;
    this->rot = Vec3D();
    this->mat = material; 
    this->age = age;
}

void Particle::update(Vec3D gravity, bool friction, Point3D spheres[3], int timerVal)
{
    age--;

    Particle trailParticle = Particle(
        pos,
        Vec3D(),
        colour,
        matIndex,
        0,
        5,
        Vec3D(),
        mat,
        25
    );

    if (speed >= 0.1 && timerVal % 3 == 0)
    {
        trail.push_back(trailParticle);
    }

    if (trail.size() > 0 && trail.front().age <= 0)
    {
        trail.erase(trail.begin());
    }

    for (long long unsigned int i = 0; i < trail.size(); i++)
    {
        trail.at(i).age--;
    }

    rot = rot.add(rotInc);
    rot.x = (int)rot.x % 360; 
    rot.y = (int)rot.y % 360; 
    rot.z = (int)rot.z % 360; 

    Vec3D vel = dir.multiply(speed);
    vel = vel.add(gravity);
    pos = vel.movePoint(pos);

    if (pos.y < 0)
    {
        if (pos.x < 200 && pos.x > -200 && pos.z < 200 && pos.z > -200)
        {
            if (friction) vel = vel.multiply(0.8);
            pos.y = 0;
            vel.y = -vel.y;
        }
    }

    
    if(pos.distanceTo(spheres[0]) <= 30.0) {
        Vec3D normal = Vec3D::createVector(spheres[0], pos).normalize().multiply(30.2);

        pos = normal.movePoint(spheres[0]);    

        vel = vel.multiply(-1);
    }

    if(pos.distanceTo(spheres[1]) <= 30.0) {
        Vec3D normal = Vec3D::createVector(spheres[1], pos).normalize().multiply(30.2);

        pos = normal.movePoint(spheres[1]);    

        vel = vel.multiply(-1);
    }

    if(pos.distanceTo(spheres[2]) <= 30.0) {
        Vec3D normal = Vec3D::createVector(spheres[2], pos).normalize().multiply(30.2);

        pos = normal.movePoint(spheres[2]);

        vel = vel.multiply(-1);
    }

    dir = vel.normalize();
    speed = vel.length();

    if (speed < 0.1)
    {
        rotInc = Vec3D();
    }
}