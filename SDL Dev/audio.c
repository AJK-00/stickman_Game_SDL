#include "audio.h"
#include <stdio.h>

GameAudio audio;

bool init_audio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! Error: %s\n", Mix_GetError());
        return false;
    }

    audio.bgm = Mix_LoadMUS("audio/bgm.wav");
    if (!audio.bgm) {
        printf("Failed to load bgm.wav! Error: %s\n", Mix_GetError());
    }

    audio.jump = Mix_LoadWAV("audio/jump.wav");
    if (!audio.jump) {
        printf("Failed to load jump.wav! Error: %s\n", Mix_GetError());
    }

    audio.coin = Mix_LoadWAV("audio/coin.wav");
    if (!audio.coin) {
        printf("Failed to load coin.wav! Error: %s\n", Mix_GetError());
    }

    audio.death = Mix_LoadWAV("audio/death.wav");
    if (!audio.death) {
        printf("Failed to load death.wav! Error: %s\n", Mix_GetError());
    }

    if (audio.bgm) {
        Mix_PlayMusic(audio.bgm, -1);
    }

    return true;
}

void cleanup_audio() {
    Mix_FreeMusic(audio.bgm);
    Mix_FreeChunk(audio.jump);
    Mix_FreeChunk(audio.coin);
    Mix_FreeChunk(audio.death);
    Mix_CloseAudio();
}