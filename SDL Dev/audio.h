#ifndef AUDIO_H
#define AUDIO_H

#include "game_types.h"

extern GameAudio audio;

bool init_audio();
void cleanup_audio();

#endif