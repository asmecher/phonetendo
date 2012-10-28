#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include "SDL/SDL_video.h"
#include "SDL/SDL_mixer.h"

#include "game.h"

extern void handle_encounter(SDL_Surface *screen, character *c, int whichnpc);
extern bool init_encounters();
extern void close_encounters();

#endif
