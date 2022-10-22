#ifndef SPRING_H
#define SPRING_H

#include "raylib.h"
#include "raymath.h"
#include "particle.h"

using namespace std;

class Spring
{
    public:
        Spring();
        Spring(float k,  float springLength, Particle *a, Particle *b);
        ~Spring();
        void Update();
        void Draw();
        void SetDrawInfo(float stroke, Color color);
        Particle* GetParticleA();
        Particle* GetParticleB();
        float GetSpringLength();
    private:
        Particle* _a;
        Particle* _b;
        //Spring constant
        float _k;
        //Spring length not effected by weight
        float _springLength;
        //extention
        float _x = 0;
        //spring force
        Vector2 _force = Vector2Zero();

        //Draw
        float _stroke = 2;
        Color _color = BLACK;
};

Spring::Spring(float k, float springLength, Particle *a, Particle *b)
{
    _k = k;
    _springLength = springLength;
    _a = a;
    _b = b;
}

//Delete pointers
Spring::~Spring()
{
    delete(_a);
    delete(_b);
}

void Spring::Update()
{
    //Get vector from a to b
    Vector2 vec = Vector2Subtract(_b->GetPosition(), _a->GetPosition());
    //Get extention
    _x = (Vector2Magnitude(vec) - _springLength);
    //get direction
    _force = Vector2Normalize(vec);
    //apply F = -k * x * dir
    //Note: we don't -_k straight away, because one particle should go to other position
    _force = Vector2Scale(_force, _k * _x);
    //add force to particles
    _a->AddForce(_force);
    //reverse direction
    _force = Vector2Scale(_force, -1);
    //aad force to another particle
    _b->AddForce(_force);
}

//Draw spring
void Spring::Draw()
{
    DrawLineEx(_a->GetPosition(), _b->GetPosition(), _stroke, _color);
}

//Sets spring draw info
void Spring::SetDrawInfo(float stroke, Color color)
{
    _stroke = stroke;
    _color = color;
}

//Returns particle a, that is connected to spring
Particle* Spring::GetParticleA()
{
    return _a;
}

//Returns particle b, that is connected to spring
Particle* Spring::GetParticleB()
{
    return _b;
}

//Returns spring length, between connected particles
float Spring::GetSpringLength()
{
    Vector2 vec = Vector2Subtract(_b->GetPosition(), _a->GetPosition()); 
    return Vector2Length(vec);
}

#endif