#ifndef NPC_H
#define NPC_H

#include "SDL/SDL_video.h"
#include "SDL/SDL_ttf.h"

#include "drawlist.h"
#include "route.h"

const int ENCOUNTER_DELAY = 30;
const int NPCCOUNT=18;

const float NPC_SPEED_MIN = 0.025;
const float NPC_SPEED_VARIATION = 0.05;

extern const char *npcnames[NPCCOUNT];

typedef struct {
	pathhead *route;
	pathnode *location;
	int i,j, lasti, lastj;
	int curindex;
	float speed;
	float progress;
	drawlistentry *d;
	unsigned char orientation;
	int lastencounter;
	SDL_Surface *nametag;
} NPC;

extern void init_npcs(TTF_Font *font);
extern void close_npcs();

extern NPC npcs[NPCCOUNT];

#endif
