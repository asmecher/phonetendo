#ifndef STAPLER_H
#define STAPLER_H

#include "tiles.h"
#include "drawlist.h"

typedef struct {
	int i,j;
	bool placed;
	tileset *t;
	drawlistentry *d;
} STAPLER;

extern STAPLER *create_stapler();
extern void place_stapler(drawlistheads dh, STAPLER *s, int i, int j);
extern void unplace_stapler(drawlistheads dh, STAPLER *s);

extern void destroy_stapler(STAPLER *s);

#endif
