#include "office.h"
#include "controller.h"
#include "route.h"
#include "game.h"
#include "umec_device.h"

int main(int argc, char* args[]) {
	if (!controller_open()) {
		fprintf(stderr, "Unable to open keypad.\n");
		return -1;
	}
	if (!open_umec()) {
		fprintf(stderr, "Unable to open umec device.\n");
		return -1;
	}
	fprintf(stderr, "Controller open\n");
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) { // | SDL_INIT_AUDIO
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return -1;
	}

	fprintf(stderr, "SDL initialized\n");
	// SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	SDL_Surface *screen = SDL_SetVideoMode(0, 0, 0, SDL_SWSURFACE);
	fprintf(stderr, "Video mode set\n");
	TTF_Init();
	fprintf(stderr, "TTF initialized\n");
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 4096) != 0) {
		fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
		return -1;
	}
	Mix_AllocateChannels(3);

	fprintf(stderr, "About to launch game\n");
	game(screen);

	// Shut down
	SDL_Quit();
	TTF_Quit();
	Mix_CloseAudio();
	controller_close();
	close_umec();

	return 0;
}
