#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

struct Battery {
    int id;
    int velocity;
    SDL_Rect layout;
} Battery;

int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    };

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );

    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return FALSE;
    }
    return TRUE;
}

void process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            game_is_running = FALSE;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                game_is_running = FALSE;
            break;
    }
}

void setup() {
    Battery.velocity = 100;
    Battery.layout.x = 100;
    Battery.layout.y = 100;
    Battery.layout.w = 80;
    Battery.layout.h = 50;
}

SDL_Rect setup_left_ground() {
    SDL_Rect left_ground;
    left_ground.x = 0;
    left_ground.y = WINDOW_HEIGHT - 100;
    left_ground.w = 400;
    left_ground.h = 100;

    return left_ground;
}

SDL_Rect setup_right_ground() {
    SDL_Rect right_ground;
    right_ground.x = 550;
    right_ground.y = WINDOW_HEIGHT - 100;
    right_ground.w = 500;
    right_ground.h = 200;

    return right_ground;
}

SDL_Rect setup_bridge() {
    SDL_Rect bridge;
    bridge.x = 375;
    bridge.y = WINDOW_HEIGHT - 100;
    bridge.w = 200;
    bridge.h = 20;

    return bridge;
}

void update() {
    // Fator que será utilizado para calcular a mudança de pixels a cada segundo.
    // Utilizando esse fator não é preciso deduzir a mudança a cada frame.
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();

    Battery.layout.x += Battery.velocity * delta_time;
    Battery.layout.y += Battery.velocity * delta_time;
}

void render(SDL_Rect left_ground, SDL_Rect right_ground, SDL_Rect bridge) {
    // Colorindo o fundo da tela.
    SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 93, 120, 16, 255);
    SDL_RenderFillRect(renderer, &Battery.layout);

    SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
    SDL_RenderFillRect(renderer, &left_ground);
    SDL_RenderFillRect(renderer, &right_ground);
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    SDL_RenderFillRect(renderer, &bridge);

    SDL_RenderPresent(renderer);
}

void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main () {
    game_is_running = initialize_window();

    setup();

    SDL_Rect left_ground = setup_left_ground();
    SDL_Rect right_ground = setup_right_ground();
    SDL_Rect bridge = setup_bridge();

    while(game_is_running) {
        process_input();
        update();
        render(left_ground, right_ground, bridge);
    }

    destroy_window();

    return 0;
}