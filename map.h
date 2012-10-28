#ifndef MAP_H
#define MAP_H

const int MAP_WIDTH=40, MAP_HEIGHT=40;

extern int distancesquared(int i1, int j1, int i2, int j2);
extern void getRandomOpenSpot(int *targeti, int *targetj, bool visible = false);

extern const unsigned char floor[MAP_WIDTH*MAP_HEIGHT];
extern const unsigned char map[MAP_WIDTH*MAP_HEIGHT];

#endif
