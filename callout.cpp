#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_timer.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include "game.h"
#include "callout.h"
#include "umec_device.h"
#include "controller.h"
#include "dtmf.h"

void handle_callout(SDL_Surface *screen, int speaker) {
	int oldhookstate = get_hook_state();

	Mix_HaltChannel(MUZAK_CHANNEL);

	memset(screen->pixels, 0, 640*480*2); // 16bpp=2bytes
	SDL_Surface *text = TTF_RenderText_Solid(fontLarge, "Outgoing Call", fontcolour);
	SDL_Rect textrect = {180, 200, 0, 0};
	SDL_BlitSurface(text, NULL, screen, &textrect);
	SDL_Flip(screen);
	SDL_FreeSurface(text);

	set_audio_output(speaker);

	Mix_CloseAudio();
	Mix_OpenAudio(8000, MIX_DEFAULT_FORMAT, 1, 512);
	Mix_AllocateChannels(1);

	if (!open_dtmf()) {
		fprintf(stderr, "Unable to open DTMF files!\n");
	}

	bool leave = false;
	Mix_PlayChannel(0, dialtone, -1);

	while (!leave) {
		int newhookstate = get_hook_state();
		struct input_event *i = controller_read();
		if (i != NULL && i->type == 1) {
			switch (i->value) {
				case PKEY_PRESS: switch (i->code) {
					case PKEY_SPEAKER:
						set_audio_output(speaker = 1-speaker);
						break;
					case PKEY_1: Mix_PlayChannel(0, dtmf[1], -1); break;
					case PKEY_2: Mix_PlayChannel(0, dtmf[2], -1); break;
					case PKEY_3: Mix_PlayChannel(0, dtmf[3], -1); break;
					case PKEY_4: Mix_PlayChannel(0, dtmf[4], -1); break;
					case PKEY_5: Mix_PlayChannel(0, dtmf[5], -1); break;
					case PKEY_6: Mix_PlayChannel(0, dtmf[6], -1); break;
					case PKEY_7: Mix_PlayChannel(0, dtmf[7], -1); break;
					case PKEY_8: Mix_PlayChannel(0, dtmf[8], -1); break;
					case PKEY_9: Mix_PlayChannel(0, dtmf[9], -1); break;
					case PKEY_0: Mix_PlayChannel(0, dtmf[0], -1); break;
					case PKEY_POUND: Mix_PlayChannel(0, dtmf[DTMF_POUND], -1); break;
					case PKEY_STAR: Mix_PlayChannel(0, dtmf[DTMF_STAR], -1); break;
				} break;
				case PKEY_RELEASE: switch (i->code) {
					case PKEY_1:
					case PKEY_2:
					case PKEY_3:
					case PKEY_4:
					case PKEY_5:
					case PKEY_6:
					case PKEY_7:
					case PKEY_8:
					case PKEY_9:
					case PKEY_0:
					case PKEY_POUND:
					case PKEY_STAR:
						Mix_HaltChannel(0);
						break;
				} break;
			}
		}
		if (speaker == HANDSET && newhookstate == ON_HOOK) leave = true;
		else if (oldhookstate != newhookstate) {
			set_audio_output(speaker = 1-speaker);
			oldhookstate = newhookstate;
		}
	}

	close_dtmf();

	Mix_CloseAudio();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 4096);
	Mix_AllocateChannels(3);

	set_audio_output(SPEAKER);
	Mix_PlayChannel(MUZAK_CHANNEL, muzak, -1);
}


