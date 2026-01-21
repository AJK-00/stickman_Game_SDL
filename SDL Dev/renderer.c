#include "renderer.h"
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

bool isDay = true;
int total_score = 0;

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

void draw_stars(SDL_Renderer* renderer, float world_pos_x) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < STAR_COUNT; i++) {
        int star_x = ((i * 37) + (int)(world_pos_x / 3)) % WINDOW_WIDTH;
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

    SDL_RenderDrawLine(renderer, x + MAN_WIDTH / 2, y + 10, x + MAN_WIDTH / 2, y + MAN_HEIGHT - 10);

    int head_x = x + MAN_WIDTH / 2;
    int head_y = y + 5;
    for (int dx = -5; dx <= 5; dx++) {
        for (int dy = -5; dy <= 5; dy++) {
            if (dx * dx + dy * dy <= 25) {
                SDL_RenderDrawPoint(renderer, head_x + dx, head_y + dy);
            }
        }
    }

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

    const char* text = "GAME OVER";
    for (int i = 0; text[i]; i++) {
        draw_digit(renderer, text[i] - '0', x + i * 20, y);
    }

    y = box.y + box.h - 60;
    x = box.x + 30;

    char score_text[32];
    snprintf(score_text, sizeof(score_text), "Score: %d", total_score);
    for (int i = 0; score_text[i]; i++) {
        if (score_text[i] >= '0' && score_text[i] <= '9') {
            draw_digit(renderer, score_text[i] - '0', x + i * 20, y);
        }
    }
}