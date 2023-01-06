#include "app.h"
#include "fsm.h"
#include "asset_man.h"

#include <stdio.h>

struct app_t app;

void app_init(const app_options_t* options)
{
    app.options = *options;

    app.should_close = false;
    app.window = SDL_CreateWindow(
        "Space Invaders Clone",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        app.options.scale * APP_VSCREEN_WIDTH,
        app.options.scale * APP_VSCREEN_HEIGHT,
        options->fullscreen ? SDL_WINDOW_FULLSCREEN : 0
    );
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    
    app.frame_time = 0;

    asset_man_init();
    fsm_init();
}

void app_destroy()
{
    // destroy all states properly because we're quitting the app.
    fsm_destroy();
    // destroy all resources
    asset_man_destroy();
    
    // app stuff
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
}

void app_run()
{
    uint32_t before = SDL_GetTicks();

    while (!app.should_close)
    {
        // calculate frame deltatime
        app.frame_time = SDL_GetTicks() - before;
        if (app.frame_time < 16) // only finish frame if fps is around 60
            continue;

        // now a new frame started
        before = SDL_GetTicks();

        // poll for next event in queue
        SDL_PollEvent(&app.event);
        if (app.event.type == SDL_QUIT)
            app.should_close = true;

        fsm_update_current_state();
        fsm_render_current_state();

        fsm_update();
    }
}