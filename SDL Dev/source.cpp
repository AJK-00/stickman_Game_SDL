#include <SDL.h>
#include <SDL_mixer.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 750
#define MAN_WIDTH 50
#define MAN_HEIGHT 40
#define GRAVITY 0.5f
#define JUMP_FORCE -10.0f
#define MAX_OBSTACLES 1
#define INITIAL_SPEED 5.0f
#define ANIMAL_SPEED 1.5f
#define BIRD_SPEED 3.0f
#define GROUND_HEIGHT 550
#define ANIMAL_SPAWN_DISTANCE 300.0f
#define BIRD_SPAWN_DISTANCE 600.0f
#define BIRD_WIDTH 30
#define BIRD_HEIGHT 20
#define ANIMAL_WIDTH 40
#define ANIMAL_HEIGHT 30
#define MAX_ENEMIES 1
#define ROCK_HEIGHT_MAX 40
#define ROCK_HEIGHT_MIN 20
#define MIN_SPAWN_DISTANCE 200.0f
#define BIRD_Y_POSITION (GROUND_HEIGHT - 25)
#define ANIMAL_Y_POSITION (GROUND_HEIGHT - ANIMAL_HEIGHT)
#define DAY_DURATION 10000
#define MOON_SIZE 30
#define STAR_COUNT 200
#define SCORE_HEIGHT 50
#define MAX_COINS 20
#define COIN_SIZE 15
#define COIN_VALUE 10

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

Player player;
Obstacle obstacles[MAX_OBSTACLES];
Enemy enemies[MAX_ENEMIES];
GameAudio audio;
Coin coins[MAX_COINS];
bool isDay = true;
Uint32 game_start_time;
int total_score = 0;


bool init_audio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! Error: %s\n", Mix_GetError());
        return false;
    }

    // Load audio files from audio subdirectory
    audio.bgm = Mix_LoadMUS("audio/bgm.wav");
    if (!audio.bgm) {
        printf("Failed to load bgm.wav! Error: %s\n", Mix_GetError());
        // Continue without background music
    }

    audio.jump = Mix_LoadWAV("audio/jump.wav");
    if (!audio.jump) {
        printf("Failed to load jump.wav! Error: %s\n", Mix_GetError());
        // Set to NULL so we can check before playing
    }

    audio.coin = Mix_LoadWAV("audio/coin.wav");
    if (!audio.coin) {
        printf("Failed to load coin.wav! Error: %s\n", Mix_GetError());
        // Set to NULL so we can check before playing
    }

    audio.death = Mix_LoadWAV("audio/death.wav");
    if (!audio.death) {
        printf("Failed to load death.wav! Error: %s\n", Mix_GetError());
        // Set to NULL so we can check before playing
    }

    // Play background music if loaded successfully
    if (audio.bgm) {
        Mix_PlayMusic(audio.bgm, -1);
    }

    // Return true even if some audio files failed to load
    // The game can still run without audio
    return true;
}


void cleanup_audio() {
    Mix_FreeMusic(audio.bgm);
    Mix_FreeChunk(audio.jump);
    Mix_FreeChunk(audio.coin);
    Mix_FreeChunk(audio.death);
    Mix_CloseAudio();
}

void draw_digit(SDL_Renderer* renderer, int digit, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    switch (digit) {
    case 0:
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        SDL_RenderDrawLine(renderer, x, y, x, y + 15);
        SDL_RenderDrawLine(renderer, x + 10, y, x + 10, y + 15);
        SDL_RenderDrawLine(renderer, x, y + 15, x + 10, y + 15);
        break;
    case 1:
        SDL_RenderDrawLine(renderer, x + 5, y, x + 5, y + 15);
        break;
    case 2:
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        SDL_RenderDrawLine(renderer, x + 10, y, x + 10, y + 7);
        SDL_RenderDrawLine(renderer, x, y + 7, x + 10, y + 7);
        SDL_RenderDrawLine(renderer, x, y + 7, x, y + 15);
        SDL_RenderDrawLine(renderer, x, y + 15, x + 10, y + 15);
        break;
    case 3:
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        SDL_RenderDrawLine(renderer, x + 10, y, x + 10, y + 15);
        SDL_RenderDrawLine(renderer, x, y + 7, x + 10, y + 7);
        SDL_RenderDrawLine(renderer, x, y + 15, x + 10, y + 15);
        break;
    case 4:
        SDL_RenderDrawLine(renderer, x, y, x, y + 7);
        SDL_RenderDrawLine(renderer, x, y + 7, x + 10, y + 7);
        SDL_RenderDrawLine(renderer, x + 10, y, x + 10, y + 15);
        break;
    case 5:
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        SDL_RenderDrawLine(renderer, x, y, x, y + 7);
        SDL_RenderDrawLine(renderer, x, y + 7, x + 10, y + 7);
        SDL_RenderDrawLine(renderer, x + 10, y + 7, x + 10, y + 15);
        SDL_RenderDrawLine(renderer, x, y + 15, x + 10, y + 15);
        break;
    case 6:
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        SDL_RenderDrawLine(renderer, x, y, x, y + 15);
        SDL_RenderDrawLine(renderer, x, y + 7, x + 10, y + 7);
        SDL_RenderDrawLine(renderer, x + 10, y + 7, x + 10, y + 15);
        SDL_RenderDrawLine(renderer, x, y + 15, x + 10, y + 15);
        break;
    case 7:
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        SDL_RenderDrawLine(renderer, x + 10, y, x + 10, y + 15);
        break;
    case 8:
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        SDL_RenderDrawLine(renderer, x, y, x, y + 15);
        SDL_RenderDrawLine(renderer, x + 10, y, x + 10, y + 15);
        SDL_RenderDrawLine(renderer, x, y + 7, x + 10, y + 7);
        SDL_RenderDrawLine(renderer, x, y + 15, x + 10, y + 15);
        break;
    case 9:
        SDL_RenderDrawLine(renderer, x, y, x + 10, y);
        SDL_RenderDrawLine(renderer, x, y, x, y + 7);
        SDL_RenderDrawLine(renderer, x + 10, y, x + 10, y + 15);
        SDL_RenderDrawLine(renderer, x, y + 7, x + 10, y + 7);
        break;
    }
}

void draw_score(SDL_Renderer* renderer, int score) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect score_bg = { 10, 10, 200, 40 };
    SDL_RenderFillRect(renderer, &score_bg);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &score_bg);

    int x = 20;
    int y = 20;

    int temp = score;
    int digits[10];
    int count = 0;

    do {
        digits[count++] = temp % 10;
        temp /= 10;
    } while (temp > 0);

    x = 80;
    for (int i = count - 1; i >= 0; i--) {
        draw_digit(renderer, digits[i], x, y);
        x += 15;
    }

    x += 5;
    SDL_RenderDrawLine(renderer, x, y, x, y + 15);
    SDL_RenderDrawLine(renderer, x, y, x + 5, y + 7);
    SDL_RenderDrawLine(renderer, x + 5, y + 7, x + 10, y);
}

void draw_coin(SDL_Renderer* renderer, float x, float y) {
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);

    for (int w = -COIN_SIZE / 2; w < COIN_SIZE / 2; w++) {
        for (int h = -COIN_SIZE / 2; h < COIN_SIZE / 2; h++) {
            if (w * w + h * h <= (COIN_SIZE / 2) * (COIN_SIZE / 2)) {
                SDL_RenderDrawPoint(renderer, x + w + COIN_SIZE / 2, y + h + COIN_SIZE / 2);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    SDL_RenderDrawLine(renderer, x + COIN_SIZE / 2, y + 3, x + COIN_SIZE / 2, y + COIN_SIZE - 3);
}

void draw_total_score(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
    SDL_Rect score_bg = { WINDOW_WIDTH - 210, 10, 200, 40 };
    SDL_RenderFillRect(renderer, &score_bg);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &score_bg);

    int x = WINDOW_WIDTH - 190;
    int y = 20;
    int temp = total_score;
    int digits[10];
    int count = 0;

    do {
        digits[count++] = temp % 10;
        temp /= 10;
    } while (temp > 0);

    for (int i = count - 1; i >= 0; i--) {
        draw_digit(renderer, digits[i], x, y);
        x += 15;
    }

    draw_coin(renderer, WINDOW_WIDTH - 50, 20);
}

void draw_sun(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    int sun_x = 100, sun_y = 100;

    for (int w = 0; w < 40; w++) {
        for (int h = 0; h < 40; h++) {
            float dx = w - 20;
            float dy = h - 20;
            if (dx * dx + dy * dy <= 400) {
                SDL_RenderDrawPoint(renderer, sun_x + w, sun_y + h);
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        float angle = i * M_PI / 4;
        SDL_RenderDrawLine(renderer,
            sun_x + 20 + cos(angle) * 25,
            sun_y + 20 + sin(angle) * 25,
            sun_x + 20 + cos(angle) * 35,
            sun_y + 20 + sin(angle) * 35);
    }
}

void draw_moon(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    int moon_x = 100, moon_y = 100;

    for (int w = 0; w < MOON_SIZE; w++) {
        for (int h = 0; h < MOON_SIZE; h++) {
            float dx = w - MOON_SIZE / 2;
            float dy = h - MOON_SIZE / 2;
            if (dx * dx + dy * dy <= (MOON_SIZE / 2) * (MOON_SIZE / 2)) {
                SDL_RenderDrawPoint(renderer, moon_x + w, moon_y + h);
            }
        }
    }
}

void draw_stars(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < STAR_COUNT; i++) {
        int star_x = ((i * 37) + (int)(player.world_pos_x / 3)) % WINDOW_WIDTH;
        int star_y = (i * 23) % GROUND_HEIGHT;
        SDL_RenderDrawPoint(renderer, star_x, star_y);

        if (SDL_GetTicks() % (i + 1) < (i + 1) / 2) {
            SDL_RenderDrawPoint(renderer, star_x + 1, star_y);
            SDL_RenderDrawPoint(renderer, star_x, star_y + 1);
        }
    }
}

void draw_ground(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 101, 67, 33, 255);
    SDL_Rect ground = { 0, GROUND_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - GROUND_HEIGHT };
    SDL_RenderFillRect(renderer, &ground);
}

void draw_rock(SDL_Renderer* renderer, float x, float y, float w, float h) {
    SDL_SetRenderDrawColor(renderer, isDay ? 128 : 255, isDay ? 128 : 255, isDay ? 128 : 255, 255);
    SDL_Rect rock = { x, y, w, h };
    SDL_RenderFillRect(renderer, &rock);
}

void draw_player(SDL_Renderer* renderer, float x, float y, bool is_jumping) {
    SDL_SetRenderDrawColor(renderer, isDay ? 0 : 255, isDay ? 0 : 255, isDay ? 0 : 255, 255);

    // Body
    SDL_RenderDrawLine(renderer, x + MAN_WIDTH / 2, y + 10, x + MAN_WIDTH / 2, y + MAN_HEIGHT - 10);

    // Head
    int head_x = x + MAN_WIDTH / 2;
    int head_y = y + 5;
    for (int dx = -5; dx <= 5; dx++) {
        for (int dy = -5; dy <= 5; dy++) {
            if (dx * dx + dy * dy <= 25) {
                SDL_RenderDrawPoint(renderer, head_x + dx, head_y + dy);
            }
        }
    }

    // Arms and legs with animation
    float arm_angle = is_jumping ? -0.3f : sin(SDL_GetTicks() * 0.005f) * 0.5f;
    float leg_angle = is_jumping ? 0.2f : sin(SDL_GetTicks() * 0.005f) * 0.5f;

    SDL_RenderDrawLine(renderer, x + MAN_WIDTH / 2, y + 15,
        x + MAN_WIDTH / 2 + cos(arm_angle) * 15,
        y + 15 + sin(arm_angle) * 15);
    SDL_RenderDrawLine(renderer, x + MAN_WIDTH / 2, y + 15,
        x + MAN_WIDTH / 2 - cos(arm_angle) * 15,
        y + 15 + sin(arm_angle) * 15);

    SDL_RenderDrawLine(renderer, x + MAN_WIDTH / 2, y + MAN_HEIGHT - 10,
        x + MAN_WIDTH / 2 + cos(leg_angle) * 15,
        y + MAN_HEIGHT);
    SDL_RenderDrawLine(renderer, x + MAN_WIDTH / 2, y + MAN_HEIGHT - 10,
        x + MAN_WIDTH / 2 - cos(leg_angle) * 15,
        y + MAN_HEIGHT);
}

void draw_bird(SDL_Renderer* renderer, float x, float y) {
    SDL_SetRenderDrawColor(renderer, isDay ? 0 : 255, isDay ? 0 : 255, isDay ? 0 : 255, 255);
    float wing_y = sin(SDL_GetTicks() * 0.01f) * 5;

    SDL_RenderDrawLine(renderer, x, y, x + BIRD_WIDTH, y);
    SDL_RenderDrawLine(renderer, x + BIRD_WIDTH / 2, y, x + BIRD_WIDTH / 3, y - 10 + wing_y);
    SDL_RenderDrawLine(renderer, x + BIRD_WIDTH / 2, y, x + 2 * BIRD_WIDTH / 3, y - 10 + wing_y);
}

void draw_animal(SDL_Renderer* renderer, float x, float y) {
    SDL_SetRenderDrawColor(renderer, isDay ? 139 : 255, isDay ? 69 : 255, isDay ? 19 : 255, 255);
    float leg_offset = sin(SDL_GetTicks() * 0.01f) * 5;

    SDL_RenderDrawLine(renderer, x, y, x + ANIMAL_WIDTH, y);
    SDL_RenderDrawLine(renderer, x + 10, y, x + 10 + leg_offset, y + ANIMAL_HEIGHT);
    SDL_RenderDrawLine(renderer, x + ANIMAL_WIDTH - 10, y,
        x + ANIMAL_WIDTH - 10 - leg_offset, y + ANIMAL_HEIGHT);
}

void draw_game_over(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderFillRect(renderer, &overlay);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect box = { WINDOW_WIDTH / 4, WINDOW_HEIGHT / 3, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3 };
    SDL_RenderFillRect(renderer, &box);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &box);

    int x = box.x + 50;
    int y = box.y + 50;

    // Draw "GAME OVER"
    const char* text = "GAME OVER";
    for (int i = 0; text[i]; i++) {
        draw_digit(renderer, text[i] - '0', x + i * 20, y);
    }

    // Draw final score
    y = box.y + box.h - 60;
    x = box.x + 30;

    char score_text[32];
    sprintf_s(score_text, sizeof(score_text), "Score: %d", total_score);
    for (int i = 0; score_text[i]; i++) {
        if (score_text[i] >= '0' && score_text[i] <= '9') {
            draw_digit(renderer, score_text[i] - '0', x + i * 20, y);
        }
    }
}

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

    // Check coin collisions
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
            Mix_PlayChannel(-1, audio.coin, 0);
        }

        if (coins[i].x < player.world_pos_x - WINDOW_WIDTH) {
            coins[i].x = player.world_pos_x + WINDOW_WIDTH + rand() % 300;
            coins[i].y = GROUND_HEIGHT - 30 - (rand() % 100);
            coins[i].active = true;
        }
    }

    // Check obstacle and enemy collisions
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

    // Spawn new enemies
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
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
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
                    Mix_PlayChannel(-1, audio.jump, 0);
                }
            }
        }

        if (!player.is_dead) {
            player.is_dead = update_game();
            if (player.is_dead) {
                Mix_PlayChannel(-1, audio.death, 0);
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
            draw_stars(renderer);
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