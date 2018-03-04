#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int CENTER_WIDTH = SCREEN_WIDTH / 2;
const int CENTER_HEIGHT = SCREEN_HEIGHT / 2;
const SDL_Color WHITE = {255, 255, 255};  
const SDL_Color BLACK = {0, 0, 0}; 

typedef struct {
  int fontSize;
  TTF_Font* font;
  SDL_Rect rect;
  char* text;
} TextRect;

typedef struct {
  int reportAmmo;
  int coffee;
} Inventory;

typedef struct {
  int health;
  int status;
  int timeScore;
  int reportEff;
  Inventory inventory;
} Player;

typedef struct {
  int health;
  int status;
  char* name;
} Boss;

typedef struct {
  int startX, startY;
  int endX, endY;
} NPC;

typedef struct {
  char* text;
  TTF_Font* font;
  int numSceneRects;
  TextRect* currentScene;
  Player* player;
  Boss* firstBoss;
  Boss* finalBoss;
  NPC* maleOne;
  NPC* femaleOne;
  NPC* femaleTwo;
} Game;

// Forward Declarations
Game* newGame(char* text);
int initGame(void);

void loadTitleScene(Game* game);
void loadPlayScene(Game* game, SDL_Rect titleSceneRects[]);
void loadFightScene(Game* game, SDL_Rect titleSceneRects[]);
void loadGameoverScene(Game* game, SDL_Rect titleSceneRects[]);
void loadGameWinScene(Game* game, SDL_Rect titleSceneRects[]);

void cleanUp(SDL_Renderer *renderer,SDL_Window *window);
TTF_Font* loadText(const char *fileName, int fontSize);
void renderText(SDL_Renderer *renderer,TTF_Font *font,SDL_Rect rect, char *text);
void render(SDL_Renderer *renderer, Game* game);
void renderScene(SDL_Renderer *renderer,Game* game);
void updateScene(SDL_Renderer *renderer, Game* game);

// function definitions
void loadTitleScene(Game* game) {
    // title scene
  SDL_Rect titleRect;
  SDL_Rect subTitleRect;
  SDL_Rect startGameRect;

  TextRect* titleTextRect = malloc(sizeof *titleTextRect);
  TextRect* subTitleTextRect = malloc(sizeof *subTitleTextRect);
  TextRect* startGameTextRect = malloc(sizeof *startGameTextRect);

  char* titleText =  "Analyst Hero";
  char* subText =  "Click to start game";
  char* startText =  "START GAME";

  titleRect.x = CENTER_WIDTH - 100;
  titleRect.y = CENTER_HEIGHT - 150;
  titleRect.w = 200;
  titleRect.h = 100;
  titleTextRect->text = titleText;
  titleTextRect->rect = titleRect;
  titleTextRect->font = loadText("../assets/OpenSans-Bold.ttf", 100);

  subTitleRect.x = CENTER_WIDTH - 100;
  subTitleRect.y = CENTER_HEIGHT - 50 ;
  subTitleRect.w = 200;
  subTitleRect.h = 60;
  subTitleTextRect->text = subText;
  subTitleTextRect->rect = subTitleRect;
  subTitleTextRect->font = loadText("../assets/OpenSans-Bold.ttf", 40);

  startGameRect.x = CENTER_WIDTH - 100;
  startGameRect.y = CENTER_HEIGHT;
  startGameRect.w = 200;
  startGameRect.h = 70;
  startGameTextRect->text = startText;
  startGameTextRect->rect = startGameRect;
  startGameTextRect->font = loadText("../assets/OpenSans-Bold.ttf", 70);

  TextRect* titleSceneRects[3] = {titleTextRect, subTitleTextRect, startGameTextRect};
  game->numSceneRects = 3;

  for (int i = 0; i < 3; ++i) {
    game->currentScene[i] = *titleSceneRects[i];
    printf("test again: %s \n", game->currentScene[i].text);
  }
}

void loadPlayScene(Game* game, SDL_Rect playSceneRects[]) {
  // TODO: main play scene
}

void loadFightScene(Game* game, SDL_Rect titleSceneRects[]) {
  // TODO: fight scene
}

void loadGameoverScene(Game* game, SDL_Rect titleSceneRects[]) {
    // TODO: Gameover scene
}

void loadGameWinScene(Game* game, SDL_Rect titleSceneRects[]) {
    // TODO: GameWin scene
}

// Definitions
void renderText(SDL_Renderer *renderer,TTF_Font *font,SDL_Rect rect, char *text) {
  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, BLACK);

  SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  SDL_FreeSurface(surfaceMessage);

  SDL_RenderCopy(renderer, Message, NULL, &rect); 
  SDL_DestroyTexture(Message);
}

void updateScene(SDL_Renderer *renderer, Game* game) {
  // TODO:
}

void renderScene(SDL_Renderer *renderer, Game* game) {
  // render scene specific stuff here
  // SDL_Rect rect;
  // rect.x = 0;  //controls the rect's x coordinate 
  // rect.y = 0; // controls the rect's y coordinte
  // rect.w = 200; // controls the width of the rect
  // rect.h = 100; // controls the height of the rect
  for (int i = 0; i < game->numSceneRects; ++i) {
    renderText(
      renderer, 
      game->currentScene[i].font,  
      game->currentScene[i].rect, 
      game->currentScene[i].text
    );
  }
  // renderText(renderer, game->font,  rect, game->text);
}

TTF_Font* loadText(const char *fileName, int fontSize) {
  TTF_Font* font = TTF_OpenFont(fileName, fontSize); //this opens a font style and sets a size
  return font;
}

Game* newGame(char* text) {
  // TTF_Font* font = TTF_OpenFont(fileName, 24); //this opens a font style and sets a size
  // TODO: maybe add error handling here?

  // *note to self = must allocate (malloc) memory for structs
  Game* game = malloc(sizeof *game);
  Inventory* inventory = malloc(sizeof *inventory);
  Player* player = malloc(sizeof *player);
  Boss* firstBoss = malloc(sizeof *firstBoss);
  Boss* finalBoss = malloc(sizeof *finalBoss);
  NPC* maleOne = malloc(sizeof *maleOne);
  NPC* femaleOne = malloc(sizeof *femaleOne);
  NPC* femaleTwo = malloc(sizeof *femaleTwo);
  // struct st *x = malloc(sizeof *x); 

  TTF_Font* font = loadText("../assets/OpenSans-Bold.ttf", 100);

  game->text = text;
  game->font = font;
  game->player = player;
  game->firstBoss = firstBoss;
  game->finalBoss = finalBoss;
  game->maleOne = maleOne;
  game->femaleOne = femaleOne;
  game->femaleTwo = femaleTwo;
  game->currentScene  = (TextRect*) malloc(sizeof(TextRect*)*10);

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
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 
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
  loadTitleScene(game);
  // init scenes

  // main game loop
  int loop = 1;
  while(loop) {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      // handle user events
      if(event.type == SDL_QUIT) {
        loop = 0;
        free(game);
        cleanUp(renderer, window);
        return 0;
      }
    }
    // update
    render(renderer, game);
  }
  
  return 0;
};

int main(int argc, char *argv[]) {
  initGame();
};