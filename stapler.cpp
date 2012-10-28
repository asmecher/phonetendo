#include "tiles.h"
#include "stapler.h"
#include "game.h"

STAPLER *create_stapler() {
	STAPLER *s = (STAPLER *) malloc(sizeof(STAPLER));
	s->t = load_tiles("tiles/stapler.png", 30, 22, -12, -11);
	return s;
}

void place_stapler(drawlistheads dh, STAPLER *s, int i, int j) {
	int drawindex = TILE_TO_INDEX(i,j);
	s->d = (drawlistentry *) malloc(sizeof(drawlistentry));
	s->d->s = s->t->tiles[0];
	s->d->x = TILE_TO_SCREENX(i,j,TILE_WIDTH,TILE_HEIGHT) + s->t->xoff;
	s->d->y = TILE_TO_SCREENY(i,j,TILE_WIDTH,TILE_HEIGHT) + s->t->yoff;
	s->d->rel = true;
	TAILQ_INSERT_TAIL(&(dh[drawindex]), s->d, entries);
	fprintf(stderr, "Placing stapler at (%i, %i)\n", i, j);

	s->i = i; s->j = j;
	s->placed = true;
}

void unplace_stapler(drawlistheads dh, STAPLER *s) {
	s->placed = false;
	int drawindex = TILE_TO_INDEX(s->i,s->j);
	TAILQ_REMOVE(&(dh[drawindex]), s->d, entries);
	free (s->d);
}

void destroy_stapler(STAPLER *s) {
	free_tiles(s->t);
	free(s);
}

