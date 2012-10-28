#ifndef GAME_H
#define GAME_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <sys/queue.h>

#include "drawlist.h"
#include "route.h"
#include "map.h"
#include "stapler.h"

const int CUBICLE_TILE_WIDTH=64, CUBICLE_TILE_HEIGHT=128;
const int FLOOR_TILE_WIDTH=64, FLOOR_TILE_HEIGHT=32;
const int CHARACTER_TILE_WIDTH=128, CHARACTER_TILE_HEIGHT=128;

const int TILE_WIDTH=64, TILE_HEIGHT=32;

const int TIME_BETWEEN_CALLS = 30;

const int MUZAK_CHANNEL = 0;
const int EFFECTS_CHANNEL = 1;
const int RING_CHANNEL = 2;

const int WEST=0;
const int NORTHWEST=1;
const int NORTH=2;
const int NORTHEAST=3;
const int EAST=4;
const int SOUTHEAST=5;
const int SOUTH=6;
const int SOUTHWEST=7;

typedef struct {
	unsigned char step;
	unsigned char orientation;
	int i, j;
	bool has_stapler;
} character;

extern TTF_Font *fontSmall, *fontLarge;
extern SDL_Color fontcolour;

extern int game(SDL_Surface *screen);

extern STAPLER *stapler;
extern int who_wants_stapler;
extern Mix_Chunk *muzak;

extern drawlistheads drawlist;

#endif
