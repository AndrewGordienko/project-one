#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CIRCLE_RADIUS 20
#define GRAVITY 0.5
#define NUM_CIRCLES 10

// Structure to represent a circle's position and velocity
typedef struct {
    double x;
    double y;
    double vx;
    double vy;
} Circle;

// Function to draw a filled circle
void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

int main(int argc, char **argv){
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create window
    window = SDL_CreateWindow("Gravity Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Seed the random number generator
    srand(time(NULL));

    // Create an array of circles
    Circle circles[NUM_CIRCLES];
    for (int i = 0; i < NUM_CIRCLES; i++) {
        circles[i].x = rand() % (WINDOW_WIDTH - 2*CIRCLE_RADIUS) + CIRCLE_RADIUS;
        circles[i].y = rand() % (WINDOW_HEIGHT - 2*CIRCLE_RADIUS) + CIRCLE_RADIUS;
        circles[i].vx = ((double)rand() / RAND_MAX) * 2 - 1; // Random velocity between -1 and 1
        circles[i].vy = ((double)rand() / RAND_MAX) * 2 - 1; // Random velocity between -1 and 1
    }

    // Animation loop
    int circleCount = NUM_CIRCLES;
    while (circleCount > 0) {
        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        circleCount = NUM_CIRCLES; // Reset the count at the beginning of each loop iteration

        // Apply gravity towards the center
        double centerX = WINDOW_WIDTH / 2;
        double centerY = WINDOW_HEIGHT / 2;
        for (int i = 0; i < NUM_CIRCLES; i++) {
            Circle* circle = &circles[i];

            // Update the position of the circle
            circle->x += circle->vx;
            circle->y += circle->vy;

            // Calculate distance from the circle to the center of the screen
            double dx = centerX - circle->x;
            double dy = centerY - circle->y;
            double distance = sqrt(dx * dx + dy * dy);

            // Calculate the acceleration towards the center
            double acceleration = GRAVITY * distance;

            // Update the velocity components
            circle->vx += acceleration * (dx / distance);
            circle->vy += acceleration * (dy / distance);

            // Check for collision with walls
            if (circle->x - CIRCLE_RADIUS < 0 || circle->x + CIRCLE_RADIUS > WINDOW_WIDTH) {
                circle->vx *= -1; // Reverse velocity in x direction
            }
            if (circle->y - CIRCLE_RADIUS < 0 || circle->y + CIRCLE_RADIUS > WINDOW_HEIGHT) {
                circle->vy *= -1; // Reverse velocity in y direction
            }

            // Draw the circle
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set the render color to white
            drawCircle(renderer, (int)circle->x, (int)circle->y, CIRCLE_RADIUS);

            // Check if the circle is within a certain distance from the center
            if (distance < CIRCLE_RADIUS) {
                circleCount--;
            }
        }

        // Update the screen
        SDL_RenderPresent(renderer);

        // Delay to control the animation speed
        SDL_Delay(100);
    }

    // Cleanup and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}