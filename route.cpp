#include "route.h"
#include "game.h"
#include "map.h"

// Data types for internal calculations only
typedef struct pathcostnode {
	unsigned char parent;
	int g;
} pathcostnode;

const int DIR_MAP_OFFSETS[8] = { // y axis inverted
	-1, -1+MAP_WIDTH, MAP_WIDTH, 1+MAP_WIDTH, 1, 1-MAP_WIDTH, -MAP_WIDTH, -MAP_WIDTH-1
};

pathcostnode pathcalc[MAP_WIDTH*MAP_HEIGHT];
bool closedlist[MAP_WIDTH*MAP_HEIGHT];

void _initpathcalc() {
	for (int i=0; i<MAP_WIDTH*MAP_HEIGHT; i++) {
		pathcalc[i].g=-1;
		closedlist[i]=false;
	}
}

void freepath(pathhead *h) {
	if (h != NULL) {
		while (!LIST_EMPTY(h)) {
			pathnode *p = LIST_FIRST(h);
			LIST_REMOVE(p, entries);
			free(p);
		}
		free(h);
	}
}

pathhead *pathfind(int location, int destination, bool ext) {
	if (ext) {
		_initpathcalc();
		pathcalc[location].g=0;
		pathcalc[location].parent=-1;
	}

	int currentg=pathcalc[location].g;
	closedlist[location]=true;
	for (int i=0; i<8; i++) {
		int target = location + DIR_MAP_OFFSETS[i];
		if (map[target]!=0) continue;

		int *targetg = &(pathcalc[target].g);
		int newtargetg=currentg+((i%2==1)?14:10);
		if (*targetg==-1 || *targetg>newtargetg) {
			*targetg=newtargetg;
			pathcalc[target].parent=(i+4)%8;
		}
		if (!closedlist[target]) pathfind(target, destination, false);
	}

	if (ext) {
		int o = MAP_WIDTH*MAP_HEIGHT-1;
		int i=destination;

		pathhead *p = (pathhead *) malloc(sizeof(pathhead));
		LIST_INIT(p);

		pathnode *n = (pathnode *) malloc(sizeof(pathnode));

		while (true) {
			if (i == location) break;
			n = (pathnode *) malloc(sizeof(pathnode));
			n->i = i%MAP_WIDTH; n->j = MAP_WIDTH-(i/MAP_WIDTH)-1; // Y inverted
			LIST_INSERT_HEAD(p, n, entries);


			i+=DIR_MAP_OFFSETS[pathcalc[i].parent];
		}
		return p;
	}
}
