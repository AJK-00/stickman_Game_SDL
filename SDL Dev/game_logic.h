#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game_types.h"
#include "game_config.h"

extern Player player;
extern Obstacle obstacles[MAX_OBSTACLES];
extern Enemy enemies[MAX_ENEMIES];
extern Coin coins[MAX_COINS];
extern Uint32 game_start_time;

void init_game();
bool check_collision(SDL_Rect a, SDL_Rect b);
bool update_game();

#endif