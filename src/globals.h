#include <SDL2/SDL.h>

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

typedef struct {
    int id;
    int velocity;
    SDL_Rect layout;
} Battery;

Battery battery_one;
Battery battery_two;

typedef struct {
    int id;
    int velocity;
    SDL_Rect layout;
} Hostage;

Hostage hostages[NUM_HOSTAGES];

SDL_Rect left_ground;
SDL_Rect right_ground;
SDL_Rect bridge;
SDL_Rect left_tower;
SDL_Rect right_tower;
SDL_Rect helicopter;



