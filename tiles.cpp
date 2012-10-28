#include "tiles.h"

tileset *load_tiles(const char *filename, const int tilewidth, const int tileheight, int xoff, int yoff) {
	SDL_Surface *loadedImage = IMG_Load(filename);
	if (!loadedImage) {
		fprintf(stderr, "Could not load tileset %s!\n", filename);
		return NULL;
	}
	SDL_Surface *convertedImage = SDL_DisplayFormat(loadedImage);
	SDL_FreeSurface(loadedImage);

	int width = convertedImage->w / tilewidth;
	int height = convertedImage->h / tileheight;

	tileset *t = (tileset *) malloc(sizeof(tileset));
	t->tiles = (SDL_Surface **) malloc(sizeof(SDL_Surface *)*width*height);

	// Avoid masking the blit and getting nothing
	SDL_SetAlpha(convertedImage, SDL_RLEACCEL, SDL_ALPHA_OPAQUE);

	int i=0;
	SDL_Rect r;
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			t->tiles[i] = SDL_CreateRGBSurface(0, tilewidth, tileheight, convertedImage->format->BitsPerPixel, convertedImage->format->Rmask, convertedImage->format->Gmask, convertedImage->format->Bmask, convertedImage->format->Amask);
			if (t->tiles[i] == NULL) fprintf(stderr, "Error allocating tile %i\n", i);
			r.x = x*tilewidth;
			r.y = y*tileheight;
			r.w = tilewidth;
			r.h = tileheight;
			SDL_BlitSurface(convertedImage, &r, t->tiles[i], NULL);
			SDL_SetColorKey(t->tiles[i], SDL_SRCCOLORKEY | SDL_RLEACCEL, t->tiles[i]->format->Amask);
			SDL_SetAlpha(t->tiles[i], SDL_RLEACCEL, SDL_ALPHA_OPAQUE);
			i++;
		}
	}

	t->count = width*height;
	t->xoff = xoff;
	t->yoff = yoff;
	SDL_FreeSurface(convertedImage);

	return t;
}

void free_tiles(tileset *t) {
	for(int i=0;i<t->count;i++) {
		SDL_FreeSurface(t->tiles[i]);
	}
	free(t->tiles);
	free(t);
}

