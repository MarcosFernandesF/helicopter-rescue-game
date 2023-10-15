#include <SDL2/SDL.h>

pthread_t batteryThreads[2], shotThread;


int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int is_captured[NUM_HOSTAGES] = {FALSE};  
int is_delivered[NUM_HOSTAGES] = {FALSE}; 

int last_frame_time = 0;

pthread_mutex_t bridgeMutex, storageMutex;

SDL_Rect left_ground;
SDL_Rect right_ground;
SDL_Rect bridge;
SDL_Rect left_tower;
SDL_Rect right_tower;
SDL_Rect helicopter;
SDL_Rect storage;

SDL_Rect rescue_area_left = {
        L_TOWER_X + 50,
        L_TOWER_Y - 200,
        L_TOWER_W,
        200
};

SDL_Rect rescue_area_right = {
        R_TOWER_X + 50,
        R_TOWER_Y - 200,
        R_TOWER_W,
        200,
};

typedef struct {
    int id;
    int velocity;
    int ammo;
    int maxCapacity;
    int reloadTime;
    int needReload;
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

typedef struct {
    int active;
    SDL_Rect layout;
} Shot;

Shot shots[NUM_SHOTS];