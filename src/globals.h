#include <SDL2/SDL.h>

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int is_captured[NUM_HOSTAGES] = {FALSE};  
int is_delivered[NUM_HOSTAGES] = {FALSE}; 

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
    int is_captured;
    int is_delivred;
    int current_tower;
    SDL_Rect layout;
} Hostage;

int is_hostage_captured = FALSE;
Hostage hostages[NUM_HOSTAGES];

SDL_Rect left_ground;
SDL_Rect right_ground;
SDL_Rect bridge;
SDL_Rect left_tower;
SDL_Rect right_tower;
SDL_Rect helicopter;

typedef struct {
    int active;
    SDL_Rect layout;
} Shot;

Shot shots[NUM_SHOTS];