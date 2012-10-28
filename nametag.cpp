#include "nametag.h"

SDL_Surface *nametag_blank;

bool init_nametags() {
	SDL_Surface *loadedImage = IMG_Load("images/nametag.png");
	if (!loadedImage) {
		fprintf (stderr, "IMG_Load: %s\n", IMG_GetError());
		return false;
	}

	nametag_blank = SDL_DisplayFormat(loadedImage);
	SDL_FreeSurface(loadedImage);
	SDL_SetAlpha(nametag_blank, SDL_RLEACCEL, SDL_ALPHA_OPAQUE);
	return true;
}

void close_nametags() {
	SDL_FreeSurface(nametag_blank);
}

SDL_Surface *make_nametag(const char *name, TTF_Font *font) {
	SDL_Surface *tag = SDL_ConvertSurface(nametag_blank, nametag_blank->format, nametag_blank->flags);
	SDL_Color color = {0, 0, 0};
	SDL_Surface *nameSurface = TTF_RenderText_Solid(font, name, color);
	SDL_Rect drawRect = {50-(nameSurface->w/2), 18};
	SDL_BlitSurface(nameSurface, NULL, tag, &drawRect);
	return tag;
}
