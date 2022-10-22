#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include "raylib.h"
#include "raymath.h"

using namespace std;

class Particle
{
    public:
        Particle();
        Particle(float posX, float posY, bool staticParticles = false);
        ~Particle();
        void Update();
        void Draw();
        void SetPosition(float posX, float posY);
        Vector2 GetPosition();
        void AddForce(Vector2 force);
    private:
        Vector2 _position = Vector2Zero();
        Vector2 _accelaration = Vector2Zero();
        Vector2 _velocity = Vector2Zero();
        float _mass;
        //Can particle move or not
        bool _static;

        //draw variables
        Color _color;
        float _particleRadius;
};

Particle::Particle()
{
    Particle(0, 0);
}

Particle::Particle(float posX, float posY, bool staticParticles)
{
    _position = {posX, posY};
    _accelaration = Vector2Zero();
    _velocity = Vector2Zero();
    _mass = 10;
    _static = staticParticles;

    _particleRadius = 5;
    //If static set color to maroon else darkblue
    _color = _static == true ? MAROON : DARKBLUE;
}

Particle::~Particle(){}

//Add force to particle
void Particle::AddForce(Vector2 force)
{
    //if particle is static, don't add force
    if(_static)
        return;

    //calculate how velocity is change by applied force
    Vector2 newForce = Vector2Scale(force, 1/_mass);
    _accelaration = Vector2Add(_accelaration, newForce);
}

//update players location
void Particle::Update()
{
    //if particle is static, don't update
    if(_static)
        return;

    //.95, we lose 5% of our velocity every frame, thus faking drag
    _velocity = Vector2Scale(_velocity, 0.95);
    //Update velocity
    _velocity = Vector2Add(_velocity, _accelaration);
    //update position
    _position = Vector2Add(_position, _velocity);
    //reset accelaration as we already added it
    _accelaration = Vector2Zero();
}

//Set particle position
void Particle::SetPosition(float posX, float posY)
{
    _velocity = Vector2Zero();
    _position = {posX, posY};
}

//Get particle position
Vector2 Particle::GetPosition()
{
    return _position;
}

//Draw particles
void Particle::Draw()
{
    DrawCircleV(_position, _particleRadius, _color);
}

#endif