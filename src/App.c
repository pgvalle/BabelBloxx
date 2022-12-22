#include "App.h"

// Single instance
struct App* app = NULL;
struct App* GetAppInstance()
{
    return app;
}

void InitApp()
{
    // allocating memory for app struct
    app = malloc(sizeof(*app));

    // base stuff
    app->state = APPSTATE_GAMEPLAY;
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
    for (int i = 0; i < TEXINDEX_COUNT; i++)
        app->textures[i] = NULL;
    app->textures[TEXINDEX_CANNON] = IMG_LoadTexture(app->renderer,
        APP_ASSETS_BASEDIR "img/cannon.png");
    app->textures[TEXINDEX_HORDE] = IMG_LoadTexture(app->renderer,
        APP_ASSETS_BASEDIR "img/invaders.png");
}

void DestroyApp()
{
    // textures
    for (int i = 0; i < TEXINDEX_COUNT; i++)
    {
        if (app->textures[i])
            SDL_DestroyTexture(app->textures[i]);
    }

    // font
    TTF_CloseFont(app->font);

    // base stuff
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);

    free(app);
}

// states routines declaration

void UpdateGameplayState();
void RenderGameplayState();

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
        case APPSTATE_MAINMENU:
            break;
        case APPSTATE_GAMEPLAY:
            UpdateGameplayState();
            RenderGameplayState();
            break;
        case APPSTATE_GAMEOVER:
            break;
        }

        // updating frame delta time
        app->frameTime = SDL_GetTicks() - before;
        before = SDL_GetTicks();
    }
}

void RunApp()
{
    /* Initializing libraries */

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    TTF_Init();

    // Initialize app and all of its resources
    InitApp();

    MainLoop();

    // properly free everything
    DestroyApp();

    /* Quitting libraries */

    TTF_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}