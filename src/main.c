#include <stdlib.h>
#include <stdio.h>

#include "gamedata.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

void gameloop(struct GameData *data)
{
	Uint32 before = 0;

	while (!data->quit)
	{
		// compute frametime
		data->frametime = SDL_GetTicks() - before;
		before = SDL_GetTicks();

		// process events
		while (SDL_PollEvent(&data->event))
		{
			if (data->event.type == SDL_QUIT)
			{
				data->quit = true;
				return;
			}
		}

		// update game entities
		// cannon_update(data);
		invaders_update(data);		

		// rendering
		SDL_RenderClear(data->ren);
		// cannon_render(data);
		invaders_render(data);
		SDL_RenderPresent(data->ren);
	}
}

int main(int argc, char const** args)
{
	// SDL info
	{
		SDL_version version;
		// compiled
		SDL_VERSION(&version);
		printf("Compiled against SDL %d.%d.%d\n", version.major,
			version.minor, version.patch);
		// linked
		SDL_GetVersion(&version);
		printf("Linked against SDL %d.%d.%d\n", version.major,
			version.minor, version.patch);
	}

	// printf("arguments: ");
	// for (int i = 1; i < argc; i++)
	// 	printf("\"%s\" ", args[i]);
	// printf("\b\n");

	// initializing
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	struct GameData *data = malloc(sizeof(*data));
	data->quit = false;
	data->win = SDL_CreateWindow("Space Invaders Clone",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		GAME_SCALING_FACTOR*224, GAME_SCALING_FACTOR*256, 0);
	data->ren = SDL_CreateRenderer(data->win, -1, SDL_RENDERER_ACCELERATED);
	cannon_initialize(data);
	invaders_initialize(data);

	// execution
	gameloop(data);

	// Terminating
	invaders_destroy(data);
	cannon_destroy(data);
	SDL_DestroyRenderer(data->ren);
	SDL_DestroyWindow(data->win);
	free(data);

	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
