#ifndef RENDERER_H
#define RENDERER_H

#include "game_types.h"
#include "game_config.h"

extern bool isDay;
extern int total_score;

void draw_digit(SDL_Renderer* renderer, int digit, int x, int y);
void draw_score(SDL_Renderer* renderer, int score);
void draw_coin(SDL_Renderer* renderer, float x, float y);
void draw_total_score(SDL_Renderer* renderer);
void draw_sun(SDL_Renderer* renderer);
void draw_moon(SDL_Renderer* renderer);
void draw_stars(SDL_Renderer* renderer, float world_pos_x);
void draw_ground(SDL_Renderer* renderer);
void draw_rock(SDL_Renderer* renderer, float x, float y, float w, float h);
void draw_player(SDL_Renderer* renderer, float x, float y, bool is_jumping);
void draw_bird(SDL_Renderer* renderer, float x, float y);
void draw_animal(SDL_Renderer* renderer, float x, float y);
void draw_game_over(SDL_Renderer* renderer);

#endif