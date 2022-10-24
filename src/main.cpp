#include <iostream>
#include <vector>
#include "../lib/raylib.h"
#include "../lib/raymath.h"
#include "../lib/particle.h"
#include "../lib/spring.h"
#include "../lib/stb_perlin.h"

using namespace std;

int main(void) 
{ 
    //Screen variables
    const int screenWidth = 1600;
    const int screenHeight = 900;

    //Particle and spring instantiation variables
    //how many particles per row
    int rowCount= 73;
    //how many particles per column
    int colCount = 15;
    //distance between particles in x axis
    int paddingX = 20;
    //distance between particles in x axis
    int paddingY = 8;
    //ofset of particles in world position
    Vector2 particleOffset = {80, 30};
    
    InitWindow(screenWidth, screenHeight, "Cloth Simulation");

    Particle *particles[rowCount][colCount];
    //Selected particle for draging particles
    Particle *selectedParticle = NULL;
    vector<Spring*> springs;

    //spring constant, how stiff the string should be
    float springK = 0.5;
    //max spring length, if it reaches the spring breaks
    float maxSpringLength = 220;
    Vector2 gravity = {0, 3};
    //Wind variables
    Vector2 windDirection = {2, 2};
    float windScale = 0.1;
    float windOffsetSpeed = 1;

    SetTargetFPS(144);

    #pragma region Init

    //create grid of particles
    for(int x = 0; x < rowCount; x++)
    {
        for(int y = 0; y < colCount; y++)
        {
            //if it's top particles, make them static
            bool staticParticle = y == 0;

            Particle* particle = new Particle(x * paddingX + particleOffset.x, y * paddingY + particleOffset.y, staticParticle);
            particles[x][y] = particle;
        }
    }

    //create springs for each particles
    for(int x = 0; x < rowCount; x++)
    {
        for(int y = 0; y < colCount; y++)
        {
            Spring* spring1;
            Spring* spring2;

            //if right edge particle , don't add spring on particle right side
            if(x != rowCount - 1)
               spring1 = new Spring(springK, paddingX, particles[x][y], particles[x+1][y]);

            //if lower edge particles, don't add spring on the down side
            if(y != colCount - 1)
                spring2 = new Spring(springK, paddingY, particles[x][y], particles[x][y+1]);

            springs.push_back(spring1);
            springs.push_back(spring2);
        }
    }

    #pragma endregion Init

    // Main game loop
    while (!WindowShouldClose())
    {

#pragma region Update

        //Handle string cut logic
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            //iterate through strings
            for (int i = 0; i < springs.size(); i++)
            {
                if(springs[i] == NULL)
                    continue;

                //get particle positions
                Vector2 posA = springs[i]->GetParticleA()->GetPosition();
                Vector2 posB = springs[i]->GetParticleB()->GetPosition();

                //remove string if mouse collides string
                //20 - collisios treshold
                if(CheckCollisionPointLine(GetMousePosition(), posA, posB, 5))
                {
                    springs[i] = NULL;
                    delete(springs[i]);
                    break;
                }
            }
        }

        //Handle select particle click logic
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            //Some big dinstance
            float distance = 1000;
            
            //iterate through particles
            for(int x = 0; x < rowCount; x++)
            {
                for(int y = 0; y < colCount; y++)
                {
                    //get ndistance between particle and mouse
                    float newDis = Vector2Distance(particles[x][y]->GetPosition(), GetMousePosition());

                    //if the new distance is smaller then last distance, select particle
                    if(newDis < distance)
                    {
                        distance = newDis;
                        selectedParticle = particles[x][y];
                    }
                }
            }
        }

        //Handle drag particle logic
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            selectedParticle->SetPosition(GetMousePosition().x, GetMousePosition().y);
        }

        //Update strings
        for (int i = 0; i < springs.size(); i++)
        {
            if(springs[i] == NULL)
                continue;
            
            //if the string length is more than max, destroy it
            if(springs[i]->GetSpringLength() > maxSpringLength)
            {
                springs[i] = NULL;
                delete(springs[i]);
                continue;
            }

            //update string calculation
            springs[i]->Update();
        }

        //Update particles
        for(int x = 0; x < rowCount; x++)
        {
            for(int y = 0; y < colCount; y++)
            {
                //Add gravity to particles
                particles[x][y]->AddForce(gravity);
                
                //Wind force
                float noise = stb_perlin_noise3((float)x * windScale + GetTime() * windOffsetSpeed, (float)y * windScale + GetTime() * windOffsetSpeed, 0, 0, 0, 0);
                Vector2 wind = Vector2Scale(windDirection, noise);
                particles[x][y]->AddForce(wind);

                //Update particle
                particles[x][y]->Update();
            }
        }

#pragma endregion Update

#pragma region Draw

        BeginDrawing();

        ClearBackground(RAYWHITE);

        //draw spring visuals
        for(auto spring : springs)
        {
            if(spring == NULL)
                continue;

            spring->Draw();
        }

        //draw particle visuals
        for(int x = 0; x < rowCount; x++)
        {
            for(int y = 0; y < colCount; y++)
            {
                if(y == 0)
                    particles[x][y]->Draw();
            }
        }

        EndDrawing();

#pragma endregion Draw

    }

    // Close window and OpenGL context
    CloseWindow();        

    return 0;
}