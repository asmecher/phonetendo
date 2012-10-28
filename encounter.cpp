#include "encounter.h"
#include "game.h"
#include "misc.h"
#include "npc.h"

Mix_Chunk *hello1, *hello2, *hello3, *hello4;

bool init_encounters() {
	hello1 = Mix_LoadWAV("audio/hello1.wav");
	hello2 = Mix_LoadWAV("audio/hello2.wav");
	hello3 = Mix_LoadWAV("audio/hello3.wav");
	hello4 = Mix_LoadWAV("audio/hello4.wav");
	return hello1 && hello2 && hello3 && hello4;
}

void handle_encounter(SDL_Surface *screen, character *c, int whichnpc) {
	if (who_wants_stapler == whichnpc) {
		fprintf(stderr, "THANKS FOR THE STAPLER.\n"); // FIXME: Need audio.
		c->has_stapler = 0;
		who_wants_stapler = -1;
	} else if (!stapler->placed && !c->has_stapler && random_between(0, 5)==0) {
		fprintf(stderr, "%s wants the stapler.\n", npcnames[whichnpc]); // FIXME: Need audio.
		int i,j;
		getRandomOpenSpot(&i, &j, true);
		place_stapler(drawlist, stapler, i, j);
		who_wants_stapler = whichnpc;
	}
	else switch (random_between(0,3)) {
		case 0: Mix_PlayChannel(EFFECTS_CHANNEL, hello1, 0); break;
		case 1: Mix_PlayChannel(EFFECTS_CHANNEL, hello2, 0); break;
		case 2: Mix_PlayChannel(EFFECTS_CHANNEL, hello3, 0); break;
		case 3: Mix_PlayChannel(EFFECTS_CHANNEL, hello4, 0); break;
	}
}

void close_encounters() {
	Mix_FreeChunk(hello1);
	Mix_FreeChunk(hello2);
	Mix_FreeChunk(hello3);
	Mix_FreeChunk(hello4);
}
