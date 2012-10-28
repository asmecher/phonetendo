#include "npc.h"

#include "misc.h"
#include "map.h"
#include "tiles.h"
#include "nametag.h"

const char *npcnames[NPCCOUNT] = {
	"Alice", "Bob", "Carol", "Carlos", "Charlie", "Chuck", "Craig", "Dave", "Eve", "Mallory", "Oscar",
	"Peggy", "Trent", "Walter", "Arthur", "Merlin", "Paul", "Carole"
};

NPC npcs[NPCCOUNT];

void init_npcs(TTF_Font *font) {
	for (int i=0; i<NPCCOUNT; i++) {
		int targeti, targetj;
		getRandomOpenSpot(&targeti, &targetj);
		npcs[i].i=npcs[i].lasti=targeti; npcs[i].j=npcs[i].lastj=targetj;
		getRandomOpenSpot(&targeti, &targetj);
		npcs[i].route = pathfind(
			TILE_TO_INDEX(npcs[i].i, npcs[i].j),
			TILE_TO_INDEX(targeti, targetj)
		);
		npcs[i].location = LIST_FIRST(npcs[i].route);
		npcs[i].d = NULL;
		npcs[i].speed=rand() / (RAND_MAX+1.0) * NPC_SPEED_VARIATION + NPC_SPEED_MIN;
		npcs[i].lastencounter = 0;
		npcs[i].nametag = make_nametag(npcnames[i], font);
	}
}

void close_npcs() {
	for (int i=0; i<NPCCOUNT; i++) {
		SDL_FreeSurface(npcs[i].nametag);
	}
}
