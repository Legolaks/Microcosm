#include "SDL3/SDL.h"
#include <vector>
#include <cmath>
#include <iostream>
#include "Core/AutoBattleSimulation.h"

// Constants for the simulation
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int TARGET_FPS = 30;
const int FRAME_DELAY = 1000 / TARGET_FPS; // milliseconds
const int UNIT_SIZE = 20; // Size of each unit in pixels
const int GRID_WIDTH = 40;
const int GRID_HEIGHT = 30;
const int UNIT_COUNT = 5; // Number of units per color
Uint32 frameStart;
int frameTime;

// Helper function for linear interpolation
Uint8 lerp(Uint8 a, Uint8 b, float t) 
{
    return static_cast<Uint8>(a + (b - a) * t);
}

// helper clamp function (alternative to std::clamp)
float clamp(float value, float min, float max) 
{
    return std::fmax(min, std::fmin(value, max));
}

void RenderUnit(SDL_Renderer* renderer, Unit* unit)
{
    if (unit == nullptr)
    {
        return;
    }

    bool isDead = !unit->IsAlive();

    float healthPercentage = 0.0f;
    if (!isDead)
    {
        healthPercentage = static_cast<float>(unit->GetHitPoints()) / unit->GetMaxHitPoints();
        healthPercentage = clamp(healthPercentage, 0.0f, 1.0f);
    }

    Uint8 r, g, b;

    if (dynamic_cast<RedBall*>(unit))
    {
        if (isDead)
        {
            // Dead: Black
            r = 0;
            g = 0;
            b = 0;
        }
        else
        {
            // Red to White
            r = lerp(255, 255, healthPercentage);
            g = lerp(255, 0, healthPercentage);
            b = lerp(255, 0, healthPercentage);
        }
    }
    else if (dynamic_cast<BlueBall*>(unit))
    {
        if (isDead)
        {
            // Dead: Black
            r = 0;
            g = 0;
            b = 0;
        }
        else
        {
            // Blue to White
            r = lerp(255, 0, healthPercentage);
            g = lerp(255, 0, healthPercentage);
            b = lerp(255, 255, healthPercentage);
        }
    }
    else
    {
        r = 255;
        g = 255;
        b = 255;
    }

    // Set the render draw color
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    SDL_FRect rect;
    rect.x = unit->GetGridX() * UNIT_SIZE;
    rect.y = unit->GetGridY() * UNIT_SIZE;
    rect.w = UNIT_SIZE;
    rect.h = UNIT_SIZE;
    SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Auto Battle Simulation", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    AutoBattleSimulation simulation(GRID_WIDTH, GRID_HEIGHT, UNIT_COUNT);
    simulation.Initialize();

    bool running = true;
    SDL_Event event;

    while (running) 
    {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_EVENT_QUIT) 
            {
                running = false;
            }
        }

        simulation.Tick(0.1f); // Update simulation

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Clear screen to white
        SDL_RenderClear(renderer);

        // Render units
		for (RedBall* unit : simulation.GetRedUnits())
		{
			RenderUnit(renderer, unit);
		}
        for (BlueBall* unit : simulation.GetBlueUnits())
        {
			RenderUnit(renderer, unit);
        }

        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart; // Calculate frame time
        if (FRAME_DELAY > frameTime) 
        {
            SDL_Delay(FRAME_DELAY - frameTime); // Delay to maintain frame rate
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}