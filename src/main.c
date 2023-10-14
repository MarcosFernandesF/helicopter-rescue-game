#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

typedef struct {
    int id;
    int velocity;
    SDL_Rect layout;
} Battery;

typedef struct {
    int id;
    int velocity;
    SDL_Rect layout;
} Hostage;

Hostage hostages[NUM_HOSTAGES];

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

void move_battery(Battery *battery) {
    // Atualiza a posição da bateria com base na sua velocidade
    battery->layout.x += battery->velocity;

    // Verifica se a bateria atingiu a borda direita ou esquerda
    if (battery->layout.x <= (L_TOWER_X + L_TOWER_W - 20) || (battery->layout.x + BATTERY_W) >= R_TOWER_X) {
        // Inverte a direção se atingiu a borda
        battery->velocity = -battery->velocity;
    }
}

void check_collision(SDL_Rect *helicopter, SDL_Rect *left_tower, SDL_Rect *right_tower) {
    // Verifica colisão com o topo da tela
    if (helicopter->y <= 0) {
        game_is_running = FALSE;
        printf("Game Over: Helicóptero colidiu com o topo da tela!\n");
    }

    // Verifica colisão com esquerda da tela
    if (helicopter->x <= 0) {
        game_is_running = FALSE;
        printf("Game Over: Helicóptero colidiu com o limite da tela!\n");
    }

    // Verifica colisão com direita da tela.
    if ((helicopter->x + HELICOPTER_W) >= WINDOW_WIDTH) {
        game_is_running = FALSE;
        printf("Game Over: Helicóptero colidiu com o limite da tela!\n");
    }

    // Verificar colisão com a torre da esquerda
    if (SDL_HasIntersection(helicopter, left_tower)) {
        game_is_running = FALSE;
        printf("Game Over: Helicóptero colidiu com a torre da esquerda!\n");
    }

    // Verificar colisão com a torre da direita
    if (SDL_HasIntersection(helicopter, right_tower)) {
        game_is_running = FALSE;
        printf("Game Over: Helicóptero colidiu com a torre da direita!\n");
    }
}

void move_helicopter(SDL_Rect *helicopter, SDL_Rect *left_tower, SDL_Rect *right_tower) {
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

    if (keyboardState[SDL_SCANCODE_LEFT]) {
        helicopter->x -= HELICOPTER_VELOCITY;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        helicopter->x += HELICOPTER_VELOCITY;
    }
    if (keyboardState[SDL_SCANCODE_UP]) {
        helicopter->y -= HELICOPTER_VELOCITY;
    }
    if (keyboardState[SDL_SCANCODE_DOWN]) {
        helicopter->y += HELICOPTER_VELOCITY;
    }

    check_collision(helicopter, left_tower, right_tower);
}


void process_input(SDL_Rect *helicopter) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_is_running = FALSE;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game_is_running = FALSE;
                }
                break;
            default:
                break;
        }
    }
}

void setup_battery(Battery *battery) {
    int x_position = battery->id == 1 ? BATTERY_X : BATTERY_X - 450;
    battery->layout.x = x_position;
    battery->layout.y = BATTERY_Y;
    battery->layout.w = BATTERY_W;
    battery->layout.h = BATTERY_H;
    battery->velocity = BATTERY_VELOCITY;
}

SDL_Rect setup_left_ground() {
    SDL_Rect left_ground;
    left_ground.x = L_GROUND_X;
    left_ground.y = L_GROUND_Y;
    left_ground.w = L_GROUND_W;
    left_ground.h = L_GROUND_H;

    return left_ground;
}

SDL_Rect setup_right_ground() {
    SDL_Rect right_ground;
    right_ground.x = R_GROUND_X;
    right_ground.y = R_GROUND_Y;
    right_ground.w = R_GROUND_W;
    right_ground.h = R_GROUND_H;

    return right_ground;
}

SDL_Rect setup_bridge() {
    SDL_Rect bridge;
    bridge.x = BRIDGE_X;
    bridge.y = BRIDGE_Y;
    bridge.w = BRIDGE_W;
    bridge.h = BRIDGE_H;

    return bridge;
}

SDL_Rect setup_left_tower() {
    SDL_Rect left_tower;
    left_tower.x = L_TOWER_X;
    left_tower.y = L_TOWER_Y;
    left_tower.w = L_TOWER_W;
    left_tower.h = L_TOWER_H;

    return left_tower;
}

SDL_Rect setup_right_tower() {
    SDL_Rect right_tower;
    right_tower.x = R_TOWER_X;
    right_tower.y = R_TOWER_Y;
    right_tower.w = R_TOWER_W;
    right_tower.h = R_TOWER_H;

    return right_tower;
}

SDL_Rect setup_helicopter() {
    SDL_Rect helicopter;
    helicopter.x = HELICOPTER_X;
    helicopter.y = HELICOPTER_Y;
    helicopter.w = HELICOPTER_W;
    helicopter.h = HELICOPTER_H;

    return helicopter;
}

void setup_hostage(Hostage *hostage, int id) {
    hostage->id = id;
    hostage->layout.x = HOSTAGE_X + (id * 25);
    hostage->layout.y = HOSTAGE_Y;
    hostage->layout.w = HOSTAGE_W;
    hostage->layout.h = HOSTAGE_H;
}

void update() {
    // Fator que será utilizado para calcular a mudança de pixels a cada segundo.
    // Utilizando esse fator não é preciso deduzir a mudança a cada frame.
    // float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    // last_frame_time = SDL_GetTicks();
}

void render_background() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255);
    SDL_RenderClear(renderer);
}

void render_battery(Battery battery) {
    SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
    SDL_RenderFillRect(renderer, &battery.layout);
}

void render_ground(SDL_Rect left_ground, SDL_Rect right_ground) {
    SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
    SDL_RenderFillRect(renderer, &left_ground);
    SDL_RenderFillRect(renderer, &right_ground);
}

void render_bridge(SDL_Rect bridge) {
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    SDL_RenderFillRect(renderer, &bridge);
}

void render_towers(SDL_Rect left_tower, SDL_Rect right_tower) {
    SDL_SetRenderDrawColor(renderer, 102, 51, 0, 255);
    SDL_RenderFillRect(renderer, &left_tower);
    SDL_RenderFillRect(renderer, &right_tower);
}

void render_helicopter(SDL_Rect helicopter) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &helicopter);
}

void render_hostage(Hostage hostage) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Cor vermelha para reféns
    SDL_RenderFillRect(renderer, &hostage.layout);
}

void render_hostages() {
    for (int i = 0; i < NUM_HOSTAGES; ++i) {
        render_hostage(hostages[i]);
    }
}

void render(Battery battery_one, Battery battery_two, SDL_Rect left_ground, SDL_Rect right_ground,
    SDL_Rect bridge, SDL_Rect left_tower, SDL_Rect right_tower, SDL_Rect helicopter) {
    render_background();
    render_ground(right_ground, left_ground);
    render_bridge(bridge);
    render_towers(left_tower, right_tower);
    render_battery(battery_one);
    render_battery(battery_two);
    render_helicopter(helicopter);
    render_hostages();
    SDL_RenderPresent(renderer);
}

void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main () {
    game_is_running = initialize_window();

    Battery battery_one, battery_two;
    battery_one.id = 1;
    battery_two.id = 2;
    setup_battery(&battery_one);
    setup_battery(&battery_two);

    SDL_Rect left_ground = setup_left_ground();
    SDL_Rect right_ground = setup_right_ground();
    SDL_Rect bridge = setup_bridge();
    SDL_Rect left_tower = setup_left_tower();
    SDL_Rect right_tower = setup_right_tower();
    SDL_Rect helicopter = setup_helicopter();

    for (int i = 0; i < NUM_HOSTAGES; i++) {
        setup_hostage(&hostages[i], i);
    }

    while(game_is_running) {
        process_input(&helicopter);
        move_battery(&battery_one);
        move_battery(&battery_two);
        move_helicopter(&helicopter, &left_tower, &right_tower);
        update();
        render(battery_one, battery_two, left_ground, right_ground, bridge, left_tower, right_tower, helicopter);
    }

    destroy_window();

    return 0;
}