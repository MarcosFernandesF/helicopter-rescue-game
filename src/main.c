#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "./constants.h"
#include "./globals.h"

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

void fire(Battery *battery) {
    for (int i = 0; i < NUM_SHOTS; ++i) {
        if (!shots[i].active) {
            shots[i].active = TRUE;
            shots[i].layout.x = battery->layout.x + (BATTERY_W - SHOT_W) / 2;
            shots[i].layout.y = battery->layout.y - SHOT_H;
            break;
        }
    }
}

void move_battery(Battery *battery) {
    // Atualiza a posição da bateria com base na sua velocidade
    battery->layout.x += battery->velocity;

    // Verifica se a bateria atingiu a borda direita ou esquerda
    if (battery->layout.x <= (L_TOWER_X + L_TOWER_W - 20) || (battery->layout.x + BATTERY_W) >= R_TOWER_X) {
        // Inverte a direção se atingiu a borda
        battery->velocity = -battery->velocity;
    }
    if (rand() % 50 == 0) {  // Dispare aleatoriamente
        fire(battery);
    }
}

void check_collision() {
    // Verifica colisão com o topo da tela
    if (helicopter.y <= 0) {
        game_is_running = FALSE;
        printf("Game Over: Helicóptero colidiu com o topo da tela!\n");
    }

    // Verifica colisão com esquerda da tela
    if (helicopter.x <= 0) {
        game_is_running = FALSE;
        printf("Game Over: Helicóptero colidiu com o limite da tela!\n");
    }

    // Verifica colisão com direita da tela.
    if ((helicopter.x + HELICOPTER_W) >= WINDOW_WIDTH) {
        game_is_running = FALSE;
        printf("Game Over: Helicóptero colidiu com o limite da tela!\n");
    }

    // Verificar colisão com a torre da esquerda
    if (SDL_HasIntersection(&helicopter, &left_tower)) {
        game_is_running = FALSE;
        printf("Game Over: Helicóptero colidiu com a torre da esquerda!\n");
    }

    // Verificar colisão com a torre da direita
    if (SDL_HasIntersection(&helicopter, &right_tower)) {
        game_is_running = FALSE;
        printf("Game Over: Helicóptero colidiu com a torre da direita!\n");
    }

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

    // Verifique se o helicóptero está na área acima da torre da esquerda e pegue um refém
    if (SDL_HasIntersection(&helicopter, &rescue_area_left)) {
        if (!is_hostage_captured)
        {    
            for (int i = 0; i < NUM_HOSTAGES; ++i) {
                if (!is_captured[i]) {
                    printf("Refém %d capturado!\n", i);
                    is_captured[i] = TRUE;
                    hostages[i].current_tower = 1;  // Defina a torre atual do refém como a torre da esquerda
                    is_hostage_captured = TRUE;
                    break;
                }
            }
        }
    }

    // Se o helicóptero está na área acima da torre da direita e está carregando um refém, libere-o
    if (SDL_HasIntersection(&helicopter, &rescue_area_right)) {
        if (is_hostage_captured)
        {
            for (int i = 0; i < NUM_HOSTAGES; ++i) {
                if (is_captured[i] && !is_delivered[i]) {
                    printf("Refém %d entregue com sucesso na torre da direita!\n", i);
                    is_delivered[i] = TRUE;
                    hostages[i].current_tower = 2;  // Defina a torre atual do refém como a torre da esquerda
                    is_hostage_captured = FALSE;
                    break;
                }
            }
        }
    }
}

void move_helicopter() {
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

    if (keyboardState[SDL_SCANCODE_LEFT]) {
        helicopter.x -= HELICOPTER_VELOCITY;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        helicopter.x += HELICOPTER_VELOCITY;
    }
    if (keyboardState[SDL_SCANCODE_UP]) {
        helicopter.y -= HELICOPTER_VELOCITY;
    }
    if (keyboardState[SDL_SCANCODE_DOWN]) {
        helicopter.y += HELICOPTER_VELOCITY;
    }

    check_collision(helicopter, left_tower, right_tower);
}

void process_input() {
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

void setup_left_ground() {
    left_ground.x = L_GROUND_X;
    left_ground.y = L_GROUND_Y;
    left_ground.w = L_GROUND_W;
    left_ground.h = L_GROUND_H;
}

void setup_right_ground() {
    right_ground.x = R_GROUND_X;
    right_ground.y = R_GROUND_Y;
    right_ground.w = R_GROUND_W;
    right_ground.h = R_GROUND_H;
}

void setup_bridge() {
    bridge.x = BRIDGE_X;
    bridge.y = BRIDGE_Y;
    bridge.w = BRIDGE_W;
    bridge.h = BRIDGE_H;
}

void setup_left_tower() {
    left_tower.x = L_TOWER_X;
    left_tower.y = L_TOWER_Y;
    left_tower.w = L_TOWER_W;
    left_tower.h = L_TOWER_H;
}

void setup_right_tower() {
    right_tower.x = R_TOWER_X;
    right_tower.y = R_TOWER_Y;
    right_tower.w = R_TOWER_W;
    right_tower.h = R_TOWER_H;
}

void setup_helicopter() {
    helicopter.x = HELICOPTER_X;
    helicopter.y = HELICOPTER_Y;
    helicopter.w = HELICOPTER_W;
    helicopter.h = HELICOPTER_H;
}

void setup_hostage(Hostage *hostage, int id) {
    hostage->id = id;
    hostage->layout.x = HOSTAGE_X + (id * 25);
    hostage->layout.y = HOSTAGE_Y;
    hostage->layout.w = HOSTAGE_W;
    hostage->layout.h = HOSTAGE_H;
    hostage->is_captured = FALSE;
    hostage->is_delivred = FALSE;
    hostage->current_tower = 1;
}

void setup_shots() {
    for (int i = 0; i < NUM_SHOTS; ++i) {
        shots[i].active = FALSE;
        shots[i].layout.w = SHOT_W;
        shots[i].layout.h = SHOT_H;
    }
}

void move_shots() {
    for (int i = 0; i < NUM_SHOTS; ++i) {
        if (shots[i].active) {
            shots[i].layout.y -= SHOT_VELOCITY;  // Movendo para cima (pode ajustar conforme necessário)

            // Verificar colisão com o helicóptero
            if (SDL_HasIntersection(&shots[i].layout, &helicopter)) {
                game_is_running = FALSE;
                printf("Game Over: Helicóptero atingido por um tiro!\n");
            }

            // Verificar se o tiro saiu da tela
            if (shots[i].layout.y < 0) {
                shots[i].active = FALSE;
            }
        }
    }
}

void update() {
    // Fator que será utilizado para calcular a mudança de pixels a cada segundo.
    // Utilizando esse fator não é preciso deduzir a mudança a cada frame.
    // float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    // last_frame_time = SDL_GetTicks();
    move_shots();
}

void render_background() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255);
    SDL_RenderClear(renderer);
}

void render_batterys() {
    SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
    SDL_RenderFillRect(renderer, &battery_one.layout);
    SDL_RenderFillRect(renderer, &battery_two.layout);
}

void render_ground() {
    SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
    SDL_RenderFillRect(renderer, &left_ground);
    SDL_RenderFillRect(renderer, &right_ground);
}

void render_bridge() {
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    SDL_RenderFillRect(renderer, &bridge);
}

void render_towers() {
    SDL_SetRenderDrawColor(renderer, 102, 51, 0, 255);
    SDL_RenderFillRect(renderer, &left_tower);
    SDL_RenderFillRect(renderer, &right_tower);
}

void render_helicopter() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &helicopter);
}

void render_hostage(Hostage hostage) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    if (!is_captured[hostage.id]) {
        SDL_RenderFillRect(renderer, &hostage.layout);
    }
    if (is_delivered[hostage.id]) {
            // Verifica se o refém não foi entregue antes de renderizar na torre da direita
            SDL_Rect render_position = {
                hostage.layout.x + (R_TOWER_X - L_TOWER_X),  // Ajuste a posição X para a torre da direita
                hostage.layout.y,
                HOSTAGE_W,
                HOSTAGE_H
            };
            SDL_RenderFillRect(renderer, &render_position);
    }
}

void render_hostages() {
    for (int i = 0; i < NUM_HOSTAGES; ++i) {
        render_hostage(hostages[i]);
    }
}

void render_shots() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Cor vermelha para os tiros

    for (int i = 0; i < NUM_SHOTS; ++i) {
        if (shots[i].active) {
            SDL_RenderFillRect(renderer, &shots[i].layout);
        }
    }
}

void render() {
    render_background();
    render_ground();
    render_bridge();
    render_towers();
    render_batterys();
    render_helicopter();
    render_hostages();
    render_shots();
    SDL_RenderPresent(renderer);
}

void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main () {
    game_is_running = initialize_window();

    battery_one.id = 1;
    battery_two.id = 2;

    setup_battery(&battery_one);
    setup_battery(&battery_two);
    setup_left_ground();
    setup_right_ground();
    setup_bridge();
    setup_left_tower();
    setup_right_tower();
    setup_helicopter();
    setup_shots();
    for (int i = 0; i < NUM_HOSTAGES; i++) {
        setup_hostage(&hostages[i], i);
    }

    while(game_is_running) {
        process_input();
        move_battery(&battery_one);
        move_battery(&battery_two);
        move_helicopter();
        update();
        render();
    }

    destroy_window();

    return 0;
}