#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <stdbool.h>

typedef struct {
    float x, y;
    float vel_y;
    bool is_jumping;
    float world_pos_x;
    bool is_dead;
    int score;
} Player;

typedef struct {
    float x;
    bool is_bird;
    bool active;
} Enemy;

typedef struct {
    float x, w, h;
    bool active;
} Obstacle;

typedef struct {
    Mix_Music* bgm;
    Mix_Chunk* jump;
    Mix_Chunk* coin;
    Mix_Chunk* death;
} GameAudio;

typedef struct {
    float x, y;
    bool active;
} Coin;

#endif