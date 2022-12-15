#include <SDL2/SDL.h>

#include <stdint.h>
#include <stdbool.h>

typedef struct ClipTexture
{
	SDL_Texture *tex;
	SDL_Rect clips[10];
	int clip_count;
} ClipTexture;

#define GAME_SCALING_FACTOR 2

// 5 rows with 11 invaders each
#define INVADERS_COUNT 55

// invader types
#define INVADER0 0   // special one
#define INVADER1 1
#define INVADER2 2
#define INVADER3 3

// ~~ 60fps
#define EVENT_TIMEOUT_MS 17

struct CannonData;
struct InvaderInstance;
struct InvadersData;
struct ShieldsData;

typedef struct GameData
{
	bool quit;
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Event event;
	int64_t event_timeout;

	// texture data
	SDL_Texture *inv0,
				*inv1,
				*inv2,
				*inv3,
				*death123;

	struct CannonData
	{
		SDL_Texture *tex;
		SDL_Rect clip;
		int x,
			y;
		int64_t shot_cooldown;
		int lives;
	} cannon;

	struct InvadersData
	{
		struct InvaderInstance
		{
			int type;
			int x,
				y;
			bool has_been_updated;
			bool alive;
			int death_anim_timeout;
			bool has_played_death_anim;
			int anim_frame;
		} instances[INVADERS_COUNT];
		
		int anim_timeout;

		bool sideways_right;
		int sideways_moves_count;
	} invaders;
} GameData;

// invaders stuff
void invaders_initialize(GameData *data);
void invaders_destroy(GameData *data);
void invaders_update(GameData *data);
void invaders_render(GameData *data);

// cannon stuff
void cannon_initialize(GameData *data);
void cannon_destroy(GameData* data);
void cannon_processEvents(GameData *data);
void cannon_render(GameData* data);