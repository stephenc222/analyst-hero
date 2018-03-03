#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const SDL_Color White = {255, 255, 255};  
const SDL_Color BLACK = {0, 0, 0}; 

typedef struct {
  char* text;
  TTF_Font* font;
} Game;

// Forward Declarations
Game* newGame(char* text);

int initGame(void);

void cleanUp(SDL_Renderer *renderer,SDL_Window *window);

TTF_Font* loadText(const char *fileName, int fontSize);

void renderText(SDL_Renderer *renderer,TTF_Font *font,SDL_Rect rect, char *text);

void render(SDL_Renderer *renderer, Game* game);

void renderScene(SDL_Renderer *renderer,Game* game);

// Definitions
void renderText(SDL_Renderer *renderer,TTF_Font *font,SDL_Rect rect, char *text) {
  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, BLACK);

  SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  SDL_FreeSurface(surfaceMessage);

  SDL_RenderCopy(renderer, Message, NULL, &rect); 
}

void renderScene(SDL_Renderer *renderer, Game* game) {
  // render scene specific stuff here
    // TODO: move this to a better place
  SDL_Rect rect;
  rect.x = 0;  //controls the rect's x coordinate 
  rect.y = 0; // controls the rect's y coordinte
  rect.w = 100; // controls the width of the rect
  rect.h = 100; // controls the height of the rect
  renderText(renderer, game->font,  rect, game->text);
}

TTF_Font* loadText(const char *fileName, int fontSize) {
  TTF_Font* font = TTF_OpenFont(fileName, 24); //this opens a font style and sets a size
  return font;
}

Game* newGame(char* text) {
  // TTF_Font* font = TTF_OpenFont(fileName, 24); //this opens a font style and sets a size
  // TODO: maybe add error handling here?

  // *note to self = must allocate (malloc) memory for structs
  Game* game = malloc(sizeof *game);
  // struct st *x = malloc(sizeof *x); 
  TTF_Font* font = loadText("../assets/OpenSans-Regular.ttf",24);
  game->text = text;
  game->font = font;
  return game;
}

void render(SDL_Renderer *renderer, Game* game) {
  SDL_SetRenderDrawColor(renderer, 178, 232, 255, 255);
  SDL_RenderClear(renderer);
  // TODO: add scene-specific rendering here
  //  "SDL_RenderCopy" goes here
  renderScene(renderer,game);
  SDL_RenderPresent(renderer);
}

void cleanUp(SDL_Renderer *renderer,SDL_Window *window) {
  // FIXME: probably need to include destroying a texture here...
  // SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int initGame(void) {
  printf("init game!\n");

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("SDL_INIT Error: %s \n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    return 1;
  }
  
  SDL_Window *window = SDL_CreateWindow(
    "Analyst Hero - Alpha", 
    100, 
    100, 
    SCREEN_WIDTH, 
    SCREEN_HEIGHT, 
    SDL_WINDOW_RESIZABLE
  );

  if (window == NULL) {
    printf("SDL_CreateWindow Error: %s \n", SDL_GetError());
	  SDL_Quit();
	  return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    SDL_DestroyWindow(window);
    printf("SDL_CreateRenderer Error: %s \n", SDL_GetError());
    SDL_Quit();
    return 1;
  }    

  // TODO: refactor game struct initializer
  Game* game = newGame("SOME TITLE");

  // main game loop
  int loop = 1;
  while(loop) {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT) {
        loop = 0;
        free(game);
        cleanUp(renderer, window);
        return 0;
      }
    }
    render(renderer, game);
  }
  
  return 0;
};

int main(int argc, char *argv[]) {
  initGame();
};