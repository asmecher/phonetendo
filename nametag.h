#ifndef NAMETAG_H
#define NAMETAG_H

#include "SDL/SDL_video.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

extern bool init_nametags();
extern void close_nametags();
extern SDL_Surface *make_nametag(const char *name, TTF_Font *f);

#endif
