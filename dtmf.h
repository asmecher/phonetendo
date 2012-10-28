#ifndef DTMF_H
#define DTMF_H

#include "SDL/SDL_mixer.h"

extern bool open_dtmf();
extern void close_dtmf();

extern Mix_Chunk *dtmf[12];
extern Mix_Chunk *dialtone;

const unsigned char DTMF_STAR = 10;
const unsigned char DTMF_POUND = 11;

#endif
