#include "tasks.h"

#include "SDL/SDL_ttf.h"

#include "controller.h"
#include "game.h"
#include "npc.h"

void draw_task(SDL_Surface *screen, const char *text, int x, int y) {
	SDL_Surface *textSurface = TTF_RenderText_Solid(fontSmall, text, fontcolour);
	SDL_Rect tasktextrect = {x, y, 0, 0};
	SDL_BlitSurface(textSurface, NULL, screen, &tasktextrect);
	SDL_FreeSurface(textSurface);
}

void handle_tasks(SDL_Surface *screen, character *c) {
	memset(screen->pixels, 0, 640*480*2); // 16bpp=2bytes
	char task[80];
	int y=100;
	bool hasTasks=false;

	// Title
	SDL_Surface *text = TTF_RenderText_Solid(fontLarge, "Mail", fontcolour);
	SDL_Rect textrect = {280, text->h, 0, 0};
	SDL_BlitSurface(text, NULL, screen, &textrect);
	SDL_FreeSurface(text);

	int h = TTF_FontLineSkip(fontSmall);

	// Build tasks
	if (c->has_stapler) {
		sprintf(task, "Bring %s the stapler.", npcnames[who_wants_stapler]);
		draw_task(screen, task, 25, y+=h);
		hasTasks=true;
	} else if (who_wants_stapler != -1) {
		sprintf(task, "Find the stapler and bring it to %s.", npcnames[who_wants_stapler]);
		draw_task(screen, task, 25, y+=h);
		hasTasks=true;
	}

	if (!hasTasks) {
		sprintf(task, "Go about your regular duties.");
		draw_task(screen, task, 25, y+=h);
	}

	SDL_Flip(screen);

	bool leave = false;
	while (!leave) {
		struct input_event *i = controller_read();
		if (i != NULL && i->type == 1) {
			switch (i->value) {
				case PKEY_PRESS: leave = true;
			}
		}
	}
}
