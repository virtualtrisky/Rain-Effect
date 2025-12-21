#define RAINDROPS_COUNT 100

#include <SDL2/SDL.h>
#include <array>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>

#include <utils.hpp>

constexpr Vector2f WINDOW_SIZE = { 800, 600 };

SDL_Window   *window   = nullptr;
SDL_Renderer *renderer = nullptr;

bool running = false;

std::array<Raindrop, RAINDROPS_COUNT> raindrops;

bool init();
bool setup();
void quit();

void loop();
void render();

int main() {
    if (!init())  exit(1);
    if (!setup()) exit(1);

    running = true;
    loop();

    quit();
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("[C++] Raindrops", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE.x, WINDOW_SIZE.y, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Cannot create SDL Window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed create SDL Renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

bool setup() {
    srand(time(0));

    for (auto &raindrop : raindrops) {

        raindrop.position.x = rand() % (int)WINDOW_SIZE.x;
        raindrop.position.y = rand() % (int)WINDOW_SIZE.y;
        raindrop.z_index = ((rand()  % 10) + 1);
        raindrop.size = raindrop.z_index;
        raindrop.speed.x = raindrop.z_index / 1.5f;
        raindrop.speed.y = raindrop.z_index * 2.f;
    }

    std::sort(raindrops.begin(), raindrops.end(), [](const Raindrop &a, const Raindrop &b) { return a.z_index < b.z_index; });

    return true;
}

void quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void loop() {
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        for (auto &raindrop : raindrops) {
            raindrop.position.x += raindrop.speed.x;
            raindrop.position.y += raindrop.speed.y;

            if (raindrop.position.x > WINDOW_SIZE.x) raindrop.position.x = -1;
            if (raindrop.position.y > WINDOW_SIZE.y) raindrop.position.y = -raindrop.size;
        }

        render();
    }
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (const auto &raindrop : raindrops) {
        Uint8 color = (Uint8)(25.5f * raindrop.z_index);
        const Vector2f &pos = raindrop.position;

        float angleRad = std::atan2(raindrop.speed.y, raindrop.speed.x);

        SDL_SetRenderDrawColor(renderer, color, color, color, 255);
        SDL_RenderDrawLineF(renderer,
            pos.x, pos.y,
            (pos.x + std::cos(angleRad) * 5.5f), (pos.y + std::sin(angleRad) * 5.5f)
        );
    }

    SDL_RenderPresent(renderer);
}