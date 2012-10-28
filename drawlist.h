#ifndef DRAWLIST_H
#define DRAWLIST_H

#include <sys/queue.h>

#include "map.h"

typedef struct drawlistentry {
	SDL_Surface *s;
	int x,y;
	bool rel;
	TAILQ_ENTRY(drawlistentry) entries;
} drawlistentry;

TAILQ_HEAD(drawlisthead, drawlistentry);
typedef drawlisthead drawlistheads[MAP_WIDTH*MAP_HEIGHT];

#endif
