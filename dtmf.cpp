#include "dtmf.h"

bool open_dtmf() {
	dtmf[0] = Mix_LoadWAV("audio/outgoing/0.wav");
	dtmf[1] = Mix_LoadWAV("audio/outgoing/1.wav");
	dtmf[2] = Mix_LoadWAV("audio/outgoing/2.wav");
	dtmf[3] = Mix_LoadWAV("audio/outgoing/3.wav");
	dtmf[4] = Mix_LoadWAV("audio/outgoing/4.wav");
	dtmf[5] = Mix_LoadWAV("audio/outgoing/5.wav");
	dtmf[6] = Mix_LoadWAV("audio/outgoing/6.wav");
	dtmf[7] = Mix_LoadWAV("audio/outgoing/7.wav");
	dtmf[8] = Mix_LoadWAV("audio/outgoing/8.wav");
	dtmf[9] = Mix_LoadWAV("audio/outgoing/9.wav");
	dtmf[DTMF_STAR] = Mix_LoadWAV("audio/outgoing/star.wav");
	dtmf[DTMF_POUND] = Mix_LoadWAV("audio/outgoing/pound.wav");
	dialtone = Mix_LoadWAV("audio/outgoing/dialtone.wav");
	return (
		dtmf[0] && dtmf[1] && dtmf[2] && dtmf[3] && dtmf[4] && dtmf[5] &&
		dtmf[6] && dtmf[7] && dtmf[8] && dtmf[9] && dtmf[DTMF_STAR] && dtmf[DTMF_POUND] &&
		dialtone
	);
}

void close_dtmf() {
	for (int i=0; i<=9; i++) Mix_FreeChunk(dtmf[i]);
	Mix_FreeChunk(dtmf[DTMF_STAR]);
	Mix_FreeChunk(dtmf[DTMF_POUND]);
	Mix_FreeChunk(dialtone);
}

Mix_Chunk *dtmf[12];
Mix_Chunk *dialtone;
