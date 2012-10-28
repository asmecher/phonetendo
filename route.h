#ifndef ROUTE_H
#define ROUTE_H

#include <sys/queue.h>

const int CARDINAL_INCREMENT=2;
const float DIAG_INCREMENT=1.4;

// Data types for giving calculated paths to callers
typedef struct pathnode {
	int i, j;
	LIST_ENTRY(pathnode) entries;
} pathnode;

LIST_HEAD(pathhead, pathnode);

extern pathhead *pathfind(int location, int destination, bool ext=true);
extern void freepath(pathhead *h);

#endif
