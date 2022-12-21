#include "App.h"

// Single instance
struct App* app;
struct App* GetAppInstance()
{
    return app;
}

// ========================================================================= //
// LIBRARIES SETUP
// ========================================================================= //

void OpenLibraries()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    TTF_Init();
}

void CloseLibraries()
{
    TTF_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

// ========================================================================= //
// APP RESOURCES MANAGEMENT
// ========================================================================= //

void LoadResources()
{
    // allocating memory for app data
    app = malloc(sizeof(*app));

    // base stuff
    app->state = GAMEPLAY;
    app->shouldQuit = SDL_FALSE;
    app->window = SDL_CreateWindow(APP_TITLE, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT, 0);
    app->renderer = SDL_CreateRenderer(app->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    app->frameTime = 0;

    // icon
    {
        SDL_RWops* ops = SDL_RWFromFile(APP_ASSETS_BASEDIR "icon.svg", "rb");
        SDL_Surface* icon = IMG_LoadSVG_RW(ops);
        SDL_SetWindowIcon(app->window, icon);
        SDL_RWclose(ops);
        SDL_FreeSurface(icon);
    }

    // font
    app->font = TTF_OpenFont(APP_ASSETS_BASEDIR "font.ttf",
        APP_SCALE(10));

    // textures
    app->textures[TEXINDEX_CANNON] = IMG_LoadTexture(app->renderer,
        APP_ASSETS_BASEDIR "img/cannon.png");
    app->textures[TEXINDEX_HORDE] = IMG_LoadTexture(app->renderer,
        APP_ASSETS_BASEDIR "img/invaders.png");
}

void FreeResources()
{
    // textures
    for (int i = 0; i < TEXINDEX_COUNT; i++)
    {
        if (app->textures[i])
            SDL_DestroyTexture(app->textures[i]);
    }

    TTF_CloseFont(app->font); // font

    // base stuff
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);

    free(app);
}

// ========================================================================= //
// ENTRY POINT
// ========================================================================= //

void MainLoop();

int main(int argc, const char** args)
{
    OpenLibraries();
    LoadResources();

    // initializing entities
    Cannon_Initialize();
    Horde_Initialize();

    MainLoop();

    FreeResources();
    CloseLibraries();

    return 0;
}

// ========================================================================= //
// GAMEPLAY
// ========================================================================= //

void UpdateGameplayState()
{
    Horde_Update();
    Cannon_Update();
}

void RenderGameplayState()
{
    SDL_RenderClear(app->renderer);
    Horde_Render();
    Cannon_Render();
    SDL_RenderPresent(app->renderer);
}

void MainLoop()
{
    Uint32 before = SDL_GetTicks();

    while (!app->shouldQuit)
    {
        // polling events
        SDL_PollEvent(&app->event);
        // just to make sure the user can quit the app
        if (app->event.type == SDL_QUIT)
            app->shouldQuit = SDL_TRUE;
        
        switch (app->state)
        {
        case MAINMENU:
            break;
        case GAMEPLAY:
            UpdateGameplayState();
            RenderGameplayState();
            break;
        case GAMEOVER:
            break;
        }

        // updating frame delta time
        app->frameTime = SDL_GetTicks() - before;
        before = SDL_GetTicks();
    }
}