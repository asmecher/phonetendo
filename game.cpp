#include <malloc.h>
#include <time.h>

#include "office.h"
#include "tiles.h"
#include "controller.h"
#include "route.h"
#include "game.h"
#include "umec_device.h"
#include "callout.h"
#include "callin.h"
#include "encounter.h"
#include "map.h"
#include "misc.h"
#include "stapler.h"
#include "nametag.h"
#include "npc.h"
#include "tasks.h"

Mix_Chunk *muzak, *ring, *chimes;
TTF_Font *fontTiny, *fontSmall, *fontLarge;
SDL_Color fontcolour = {255,255,255,255};

STAPLER *stapler;
drawlistheads drawlist;
int who_wants_stapler=-1;

void player_enters_tile(character *c) {
	if (stapler->placed && stapler->i == c->i && stapler->j == c->j) {
		// Pick up the stapler
		unplace_stapler(drawlist, stapler);
		Mix_PlayChannel(EFFECTS_CHANNEL, chimes, 0);
		c->has_stapler = true;
	}
}

int game(SDL_Surface *screen) {
	// Load fonts
	fontTiny = TTF_OpenFont("FreeSansBold.ttf", 12);
	fontSmall = TTF_OpenFont("FreeSansBold.ttf", 20);
	fontLarge = TTF_OpenFont("FreeSansBold.ttf", 42);
	if (!fontSmall || !fontLarge || !fontTiny) {
		return -1;
	}

	// Load tilesets
	tileset *cubicles = load_tiles("tiles/cubicles.png", CUBICLE_TILE_WIDTH, CUBICLE_TILE_HEIGHT, -CUBICLE_TILE_WIDTH/2, TILE_HEIGHT-CUBICLE_TILE_HEIGHT-(TILE_HEIGHT/2));
	tileset *chair = load_tiles("tiles/chair.png", CUBICLE_TILE_WIDTH, CUBICLE_TILE_HEIGHT, 0, TILE_HEIGHT-CUBICLE_TILE_HEIGHT);
	tileset *carpet = load_tiles("tiles/carpet.png", FLOOR_TILE_WIDTH, FLOOR_TILE_HEIGHT, -TILE_WIDTH/2, -TILE_HEIGHT/2);
	tileset *zombie = load_tiles("tiles/zombie.png", CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, -64, -100);

	character player = {0, 0};
	int playerx=SCREEN_WIDTH/2, playery=SCREEN_HEIGHT/2;

	if (!init_encounters()) {
		fprintf(stderr, "Unable to initialize encounters.\n");
		return -1;
	}

	if (!init_nametags()) {
		fprintf(stderr, "Unable to initialize nametags.\n");
		return -1;
	}

	// Load some in-game sounds
	muzak = Mix_LoadWAV("audio/muzak.wav");
	ring = Mix_LoadWAV("audio/ring.wav");
	chimes = Mix_LoadWAV("audio/chimes.wav");
	if (muzak == NULL || ring == NULL || chimes == NULL) {
		fprintf(stderr, "Unable to load sample .wav file.\n");
		return -1;
	}

	bool leave = false;
	bool left = false, right = false, up = false, down = false;
	bool show_nametags = false;
	float x=-300, y=0;

	// Initialize the drawlist.
	for (int i=0; i<MAP_WIDTH*MAP_HEIGHT; i++) {
		TAILQ_INIT(&(drawlist[i]));
	}

	// Create The Stapler
	stapler = create_stapler();

	// Build the terrain part of the drawlist
	for (int i=0; i<MAP_WIDTH; i++) {
		for (int j=MAP_HEIGHT-1; j>=0; j--) {
			int drawindex = TILE_TO_INDEX(i,j);
			int xbase = TILE_TO_SCREENX(i,j,TILE_WIDTH,TILE_HEIGHT);
			int ybase = TILE_TO_SCREENY(i,j,TILE_WIDTH,TILE_HEIGHT);
			unsigned char t;
			drawlisthead *dh = &(drawlist[drawindex]);

			// Floorboards
			t = floor[drawindex];
			if (t!=0) {
				drawlistentry *d = (drawlistentry *) malloc(sizeof(drawlistentry));
				d->x = xbase+carpet->xoff; d->y = ybase+carpet->yoff;
				d->s = carpet->tiles[t-33];
				d->rel=true;
				TAILQ_INSERT_TAIL(dh, d, entries);
			}

			// Walls
			t = map[drawindex];
			if (t != 0) {
				drawlistentry *d = (drawlistentry *) malloc(sizeof(drawlistentry));
				d->x = xbase+cubicles->xoff; d->y = ybase+cubicles->yoff;
				d->s = cubicles->tiles[t-1];
				d->rel=true;
				TAILQ_INSERT_TAIL(dh, d, entries);
			}
		}
	}

	// Prepare the player drawer.
	int oldPlayerDrawIndex = -1;
	drawlistentry *playerDrawer = (drawlistentry *) malloc(sizeof(drawlistentry));
	playerDrawer->x = playerx+zombie->xoff;
	playerDrawer->y = playery+zombie->yoff;
	playerDrawer->rel=0;

	// Prepare the NPCs
	init_npcs(fontTiny);

	// Start the muzak
	Mix_PlayChannel(MUZAK_CHANNEL, muzak, -1);

	// Track the incoming call information
	int lastcalltime = time(NULL);
	bool ringing=false;

	srand(time(NULL));

	// Start the main game loop
	while (!leave) {
		int t = time(NULL);

		// If it's time for an incoming call to start ringing...
		if (t - lastcalltime > TIME_BETWEEN_CALLS && !ringing) {
			ringing = true;
			Mix_PlayChannel(RING_CHANNEL, ring, -1);
		}

		// If the handset goes off-hook...
		if (get_hook_state() == OFF_HOOK) {
			if (ringing) {
				Mix_HaltChannel(RING_CHANNEL);
				handle_callin(screen, HANDSET);
				lastcalltime = t = time(NULL);
				ringing = false;
			} else {
				handle_callout(screen, HANDSET);
				lastcalltime = t = time(NULL);
			}
		}

		// Handle keypad events
		struct input_event *i = controller_read();
		if (i != NULL && i->type == 1) {
			switch (i->value) {
				case PKEY_PRESS: switch (i->code) {
					case PKEY_OK: leave = true; break;
					case PKEY_UP: up = true; break;
					case PKEY_DOWN: down = true; break;
					case PKEY_LEFT: left = true; break;
					case PKEY_RIGHT: right = true; break;
					case PKEY_MY_CONTENT: {
						char buf[80];
						sprintf(buf, "/tmp/%i.bmp", t);
						SDL_SaveBMP(screen, buf);
					}
					case PKEY_SPEAKER:
						if (ringing) {
							Mix_HaltChannel(RING_CHANNEL);
							handle_callin(screen, SPEAKER);
							lastcalltime = t = time(NULL);
							ringing = false;
						} else {
							handle_callout(screen, SPEAKER);
						}
						break;
					case PKEY_MAIL: handle_tasks(screen, &player); break;
					case PKEY_STATUS: show_nametags = !show_nametags; break;
				} break;
				case PKEY_RELEASE: switch (i->code) {
					case PKEY_UP: up = false; break;
					case PKEY_DOWN: down = false; break;
					case PKEY_LEFT: left = false; break;
					case PKEY_RIGHT: right = false; break;
				} break;
			}
		}

		float newx = x, newy = y;
		if (down && right) {player.orientation=SOUTHEAST; newx-=DIAG_INCREMENT; newy-=DIAG_INCREMENT;}
		else if (down && left) {player.orientation=SOUTHWEST; newx+=DIAG_INCREMENT; newy-=DIAG_INCREMENT;}
		else if (up && right) {player.orientation=NORTHEAST; newx-=DIAG_INCREMENT; newy+=DIAG_INCREMENT;}
		else if (up && left) {player.orientation=NORTHWEST; newx+=DIAG_INCREMENT; newy+=DIAG_INCREMENT;}
		else if (left) {player.orientation=WEST; newx+=CARDINAL_INCREMENT;}
		else if (right) {player.orientation=EAST; newx-=CARDINAL_INCREMENT;}
		else if (up) {player.orientation=NORTH; newy+=CARDINAL_INCREMENT;}
		else if (down) {player.orientation=SOUTH; newy-=CARDINAL_INCREMENT;}

		if (left || right || up || down) player.step = (player.step + 1) % 24;

		// Move; check for collision with the wall. 32 is a magic number. FIXME.
		int trychari = SCREEN_TO_TILEI(-newx+playerx+32, -newy+playery, TILE_WIDTH, TILE_HEIGHT);
		int trycharj = SCREEN_TO_TILEJ(-newx+playerx+32, -newy+playery, TILE_WIDTH, TILE_HEIGHT);
		if (map[TILE_TO_INDEX(trychari, trycharj)] == 0) {
			x = newx; y = newy;
		};

		// Calculate where the player will be on the grid. 32 is a magic number. FIXME.
		player.i = SCREEN_TO_TILEI(-x+playerx+32, -y+playery, TILE_WIDTH, TILE_HEIGHT);
		player.j = SCREEN_TO_TILEJ(-x+playerx+32, -y+playery, TILE_WIDTH, TILE_HEIGHT);
		int playerDrawIndex = TILE_TO_INDEX(player.i, player.j);
		if (oldPlayerDrawIndex != playerDrawIndex) {
			drawlisthead *d = &(drawlist[playerDrawIndex]);
			if (oldPlayerDrawIndex != -1) TAILQ_REMOVE(&(drawlist[oldPlayerDrawIndex]), playerDrawer, entries);
			TAILQ_INSERT_TAIL(d, playerDrawer, entries);

			player_enters_tile(&player);
		}
		playerDrawer->s = zombie->tiles[player.orientation * 44 + (player.step/3) + 4];

		// Move NPCs
		for (int i=0; i<NPCCOUNT; i++) {
			npcs[i].progress += npcs[i].speed;

			if (npcs[i].progress >= 1) {
				// NPC has fully landed on a tile; set next destination.
				npcs[i].progress--;
				npcs[i].location = LIST_NEXT(npcs[i].location, entries);
				if (npcs[i].location == NULL) { // No more moves; calculate a new route
					int fromindex = TILE_TO_INDEX(npcs[i].i,npcs[i].j);
					int targeti, targetj;
					freepath(npcs[i].route);
					getRandomOpenSpot(&targeti, &targetj);
					npcs[i].route = pathfind(
						TILE_TO_INDEX(npcs[i].i, npcs[i].j),
						TILE_TO_INDEX(targeti, targetj)
					);
					npcs[i].location = LIST_FIRST(npcs[i].route);
				} else if (npcs[i].d != NULL) { // Continue along the current route
					npcs[i].lasti=npcs[i].i; npcs[i].lastj=npcs[i].j;
					npcs[i].i=npcs[i].location->i; npcs[i].j=npcs[i].location->j;
					if (npcs[i].i > npcs[i].lasti && npcs[i].j > npcs[i].lastj) npcs[i].orientation=EAST;
					else if (npcs[i].i > npcs[i].lasti && npcs[i].j < npcs[i].lastj) npcs[i].orientation=SOUTH;
					else if (npcs[i].i > npcs[i].lasti) npcs[i].orientation=SOUTHEAST;
					else if (npcs[i].i < npcs[i].lasti && npcs[i].j > npcs[i].lastj) npcs[i].orientation=NORTH;
					else if (npcs[i].i < npcs[i].lasti && npcs[i].j < npcs[i].lastj) npcs[i].orientation=WEST;
					else if (npcs[i].i < npcs[i].lasti) npcs[i].orientation=NORTHWEST;
					else if (npcs[i].j > npcs[i].lastj) npcs[i].orientation=NORTHEAST;
					else npcs[i].orientation = SOUTHWEST;
				}
			} else if (npcs[i].progress > 0.5 && npcs[i].progress - npcs[i].speed <= 0.5) {
				// NPC is just entering a new tile.
				pathnode *n = LIST_NEXT(npcs[i].location, entries);
				int newoffset = TILE_TO_INDEX(npcs[i].i, npcs[i].j);
				if (npcs[i].d != NULL) {
					TAILQ_REMOVE(&(drawlist[npcs[i].curindex]), npcs[i].d, entries);
				} else {
					npcs[i].d = (drawlistentry *) malloc(sizeof(drawlistentry));
					npcs[i].d->rel=true;
				}
				TAILQ_INSERT_TAIL(&(drawlist[newoffset]), npcs[i].d, entries);
				npcs[i].curindex = newoffset;

				// Check if the NPC has encountered the character
				if (npcs[i].lastencounter + ENCOUNTER_DELAY < t && distancesquared(npcs[i].i, npcs[i].j, player.i, player.j) <= 4) {
					handle_encounter(screen, &player, i);
					npcs[i].lastencounter = t;
				}
			}
			
			if (npcs[i].d != NULL) {
				float targeti = (npcs[i].i * npcs[i].progress) + (npcs[i].lasti * (1-npcs[i].progress));
				float targetj = (npcs[i].j * npcs[i].progress) + (npcs[i].lastj * (1-npcs[i].progress));
				npcs[i].d->x = TILE_TO_SCREENX(targeti,targetj,TILE_WIDTH,TILE_HEIGHT)+zombie->xoff;
				npcs[i].d->y = TILE_TO_SCREENY(targeti,targetj,TILE_WIDTH,TILE_HEIGHT)+zombie->yoff;
				npcs[i].d->s=zombie->tiles[44*(npcs[i].orientation%8)+4+((int) (npcs[i].progress*8)%8)];
			}
		}

		// Clear the screen
		memset(screen->pixels, 0, 640*480*2); // 16bpp=2bytes

		// Draw the drawlist
		for (int i=0; i<MAP_WIDTH; i++) {
			for (int j=MAP_HEIGHT-1; j>=0; j--) {
				drawlistentry *de;
				int drawindex = TILE_TO_INDEX(i,j);
				TAILQ_FOREACH(de, &(drawlist[drawindex]), entries) {
					SDL_Rect drawrect;
					if (de->rel) { // Relative position
						drawrect.x=de->x+x; drawrect.y=de->y+y;
					} else { // Fixed position e.g. player
						drawrect.x=de->x; drawrect.y=de->y;
					}
					SDL_BlitSurface(de->s, NULL, screen, &drawrect);
				}
			}
		}

		// Show nametags, if desired.
		if (show_nametags) {
			for (int i=0; i<NPCCOUNT; i++) if (npcs[i].d) {
				SDL_Rect drawrect = {npcs[i].d->x+x, npcs[i].d->y+y};
				SDL_BlitSurface(npcs[i].nametag, NULL, screen, &drawrect);
			}
		}

		// Blit the frame
		SDL_Flip(screen);

		// Save some junk for the next iteration.
		oldPlayerDrawIndex = playerDrawIndex;
	}

	// Shut down the muzak (and any other noise)
	Mix_HaltChannel(-1);
	Mix_FreeChunk(muzak);
	Mix_FreeChunk(ring);
	Mix_FreeChunk(chimes);

	close_encounters();
	destroy_stapler(stapler);
	close_nametags();
	close_npcs();

	// Free the drawlist entries.
	for (int i=0; i<MAP_WIDTH*MAP_HEIGHT; i++) {
		drawlisthead *d = &(drawlist[i]);
		while (!TAILQ_EMPTY(d)) {
			drawlistentry *de = TAILQ_FIRST(d);
			TAILQ_REMOVE(d, de, entries);
			free(de);
		}
	}

	// Free the NPC routes
	for (int i=0; i<NPCCOUNT; i++) freepath(npcs[i].route);

	// Clean up
	TTF_CloseFont(fontSmall);
	TTF_CloseFont(fontLarge);
	free_tiles(cubicles);
	free_tiles(chair);
	free_tiles(carpet);
	free_tiles(zombie);

	// Finished
	return 0;
}
