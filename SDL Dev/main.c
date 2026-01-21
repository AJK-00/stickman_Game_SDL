#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_config.h"
#include "game_types.h"
#include "audio.h"
#include "renderer.h"
#include "game_logic.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!init_audio()) {
        printf("Audio initialization failed!\n");
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Running Man",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        cleanup_audio();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        cleanup_audio();
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    init_game();
    bool quit = false;

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN && !player.is_jumping && !player.is_dead) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    player.vel_y = JUMP_FORCE;
                    player.is_jumping = true;
                    if (audio.jump) Mix_PlayChannel(-1, audio.jump, 0);
                }
            }
        }

        if (!player.is_dead) {
            player.is_dead = update_game();
            if (player.is_dead) {
                if (audio.death) Mix_PlayChannel(-1, audio.death, 0);
            }
        }

        isDay = ((SDL_GetTicks() - game_start_time) % (DAY_DURATION * 2)) < DAY_DURATION;

        SDL_SetRenderDrawColor(renderer,
            isDay ? 135 : 20,
            isDay ? 206 : 20,
            isDay ? 235 : 50,
            255);
        SDL_RenderClear(renderer);

        if (isDay) {
            draw_sun(renderer);
        }
        else {
            draw_stars(renderer, player.world_pos_x);
            draw_moon(renderer);
        }

        draw_ground(renderer);

        for (int i = 0; i < MAX_COINS; i++) {
            if (!coins[i].active) continue;
            float screen_x = coins[i].x - player.world_pos_x + WINDOW_WIDTH / 4;
            if (screen_x < WINDOW_WIDTH && screen_x + COIN_SIZE > 0) {
                draw_coin(renderer, screen_x, coins[i].y);
            }
        }

        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (!obstacles[i].active) continue;
            float screen_x = obstacles[i].x - player.world_pos_x + WINDOW_WIDTH / 4;
            if (screen_x < WINDOW_WIDTH && screen_x + obstacles[i].w > 0) {
                draw_rock(renderer, screen_x, GROUND_HEIGHT - obstacles[i].h,
                    obstacles[i].w, obstacles[i].h);
            }
        }

        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].active) continue;
            float screen_x = enemies[i].x - player.world_pos_x + WINDOW_WIDTH / 4;
            if (screen_x < WINDOW_WIDTH && screen_x + ANIMAL_WIDTH > 0) {
                if (enemies[i].is_bird) {
                    draw_bird(renderer, screen_x, BIRD_Y_POSITION);
                }
                else {
                    draw_animal(renderer, screen_x, ANIMAL_Y_POSITION);
                }
            }
        }

        draw_player(renderer, WINDOW_WIDTH / 4, player.y, player.is_jumping);
        draw_score(renderer, player.score);
        draw_total_score(renderer);

        if (player.is_dead) {
            draw_game_over(renderer);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    cleanup_audio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}