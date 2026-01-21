#include "game_logic.h"
#include "audio.h"
#include "renderer.h"
#include <stdlib.h>
#include <time.h>

Player player;
Obstacle obstacles[MAX_OBSTACLES];
Enemy enemies[MAX_ENEMIES];
Coin coins[MAX_COINS];
Uint32 game_start_time;

void init_game() {
    player.x = WINDOW_WIDTH / 4;
    player.y = GROUND_HEIGHT - MAN_HEIGHT;
    player.vel_y = 0;
    player.is_jumping = false;
    player.world_pos_x = 0;
    player.is_dead = false;
    player.score = 0;
    total_score = 0;
    game_start_time = SDL_GetTicks();

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].x = 800 + i * 400;
        obstacles[i].w = 30 + rand() % 20;
        obstacles[i].h = ROCK_HEIGHT_MIN + rand() % (ROCK_HEIGHT_MAX - ROCK_HEIGHT_MIN);
        obstacles[i].active = true;
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].x = 1000 + i * 500;
        enemies[i].is_bird = false;
        enemies[i].active = false;
    }

    for (int i = 0; i < MAX_COINS; i++) {
        coins[i].x = 800 + i * 100 + (rand() % 100);
        coins[i].y = GROUND_HEIGHT - 30 - (rand() % 100);
        coins[i].active = true;
    }
}

bool check_collision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y);
}

bool update_game() {
    if (player.is_jumping) {
        player.y += player.vel_y;
        player.vel_y += GRAVITY;

        if (player.y >= GROUND_HEIGHT - MAN_HEIGHT) {
            player.y = GROUND_HEIGHT - MAN_HEIGHT;
            player.vel_y = 0;
            player.is_jumping = false;
        }
    }

    player.world_pos_x += INITIAL_SPEED;
    player.score = (int)(player.world_pos_x / 10);

    SDL_Rect player_rect = {
        WINDOW_WIDTH / 4,
        (int)player.y,
        MAN_WIDTH,
        MAN_HEIGHT
    };

    for (int i = 0; i < MAX_COINS; i++) {
        if (!coins[i].active) continue;

        SDL_Rect coin_rect = {
            (int)(coins[i].x - player.world_pos_x + WINDOW_WIDTH / 4),
            (int)coins[i].y,
            COIN_SIZE,
            COIN_SIZE
        };

        if (check_collision(player_rect, coin_rect)) {
            coins[i].active = false;
            total_score += COIN_VALUE;
            if (audio.coin) Mix_PlayChannel(-1, audio.coin, 0);
        }

        if (coins[i].x < player.world_pos_x - WINDOW_WIDTH) {
            coins[i].x = player.world_pos_x + WINDOW_WIDTH + rand() % 300;
            coins[i].y = GROUND_HEIGHT - 30 - (rand() % 100);
            coins[i].active = true;
        }
    }

    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) continue;

        SDL_Rect obstacle_rect = {
            (int)(obstacles[i].x - player.world_pos_x + WINDOW_WIDTH / 4),
            GROUND_HEIGHT - (int)obstacles[i].h,
            (int)obstacles[i].w,
            (int)obstacles[i].h
        };

        if (check_collision(player_rect, obstacle_rect)) {
            return true;
        }

        if (obstacles[i].x < player.world_pos_x - WINDOW_WIDTH) {
            obstacles[i].x = player.world_pos_x + WINDOW_WIDTH + rand() % 300;
            obstacles[i].w = 30 + rand() % 20;
            obstacles[i].h = ROCK_HEIGHT_MIN + rand() % (ROCK_HEIGHT_MAX - ROCK_HEIGHT_MIN);
        }
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;

        SDL_Rect enemy_rect = {
            (int)(enemies[i].x - player.world_pos_x + WINDOW_WIDTH / 4),
            enemies[i].is_bird ? BIRD_Y_POSITION : ANIMAL_Y_POSITION,
            enemies[i].is_bird ? BIRD_WIDTH : ANIMAL_WIDTH,
            enemies[i].is_bird ? BIRD_HEIGHT : ANIMAL_HEIGHT
        };

        if (check_collision(player_rect, enemy_rect)) {
            return true;
        }

        if (enemies[i].x < player.world_pos_x - WINDOW_WIDTH) {
            enemies[i].active = false;
        }
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            if (player.world_pos_x >= BIRD_SPAWN_DISTANCE) {
                enemies[i].x = player.world_pos_x + WINDOW_WIDTH + rand() % 300;
                enemies[i].is_bird = (rand() % 2 == 0);
                enemies[i].active = true;
            }
            else if (player.world_pos_x >= ANIMAL_SPAWN_DISTANCE) {
                enemies[i].x = player.world_pos_x + WINDOW_WIDTH + rand() % 300;
                enemies[i].is_bird = false;
                enemies[i].active = true;
            }
        }
    }

    return false;
}