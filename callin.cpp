#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_timer.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include "game.h"
#include "callin.h"
#include "umec_device.h"
#include "controller.h"
#include "misc.h"
#include "dtmf.h"

void handle_callin(SDL_Surface *screen, int speaker) {
	int oldhookstate = get_hook_state();

	Mix_HaltChannel(MUZAK_CHANNEL);

	memset(screen->pixels, 0, 640*480*2); // 16bpp=2bytes
	SDL_Surface *text = TTF_RenderText_Solid(fontLarge, "Incoming Call", fontcolour);
	SDL_Rect textrect = {180, 200, 0, 0};
	SDL_BlitSurface(text, NULL, screen, &textrect);
	SDL_Flip(screen);
	SDL_FreeSurface(text);

	set_audio_output(speaker);

	switch (random_between(1,1)) { // Other cases to follow
		case 1: { // Robocall
			char buf[80];

			Mix_CloseAudio();
			Mix_OpenAudio(8000, MIX_DEFAULT_FORMAT, 1, 512);

			sprintf(buf, "audio/incoming/robocall%i-8k.wav", random_between(1,6));
			Mix_Chunk *robocall = Mix_LoadWAV(buf);
			Mix_PlayChannel(EFFECTS_CHANNEL, robocall, 0);
			bool leave = false;
			while (!leave) {
				int newhookstate = get_hook_state();
				struct input_event *i = controller_read();
				if (i != NULL && i->type == 1) {
					switch (i->value) {
						case PKEY_PRESS: switch (i->code) {
							case PKEY_SPEAKER:
								set_audio_output(speaker = 1-speaker);
								break;
						}
					}
				}
				if (speaker == HANDSET && newhookstate == ON_HOOK) leave = true;
				else if (oldhookstate != newhookstate) {
					set_audio_output(speaker = 1-speaker);
					oldhookstate = newhookstate;
				}
			}
			Mix_HaltChannel(EFFECTS_CHANNEL);
			Mix_FreeChunk(robocall);
			Mix_CloseAudio();
			Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 4096);

		} break;
	}

	set_audio_output(SPEAKER);
	Mix_PlayChannel(MUZAK_CHANNEL, muzak, -1);
}
