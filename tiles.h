#ifndef TILES_H
#define TILES_H

#include "office.h"
#include "map.h"

int inline TILE_TO_SCREENX(float i,float j,int w,int h) {
	return (j+i)*w/2;
}

int inline TILE_TO_SCREENY(float i,float j,int w,int h) {
	return (i-j)*h/2;
}

int inline SCREEN_TO_TILEI(int x,int y,int w,int h) {
	return ((x*h) + (y*w)) / (w*h);
}

int inline SCREEN_TO_TILEJ(int x,int y,int w,int h) {
	return ((x*h) - (y*w)) / (w*h);
}

int inline TILE_TO_INDEX(int i,int j) {
	return (MAP_HEIGHT-1-j)*MAP_WIDTH+i;
}

typedef struct {
	SDL_Surface **tiles;
	int xoff, yoff;
	int count;
} tileset;

extern tileset *load_tiles(const char *filename, const int tilewidth, const int tileheight, int xoff, int yoff);
extern void free_tiles(tileset *t);

#endif
