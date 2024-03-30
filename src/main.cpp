#include "app/App.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <time.h>


int main()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);

  srand(time(NULL));

  App::start();

  IMG_Quit();
  TTF_Quit();
  SDL_Quit();

  return 0;
}
