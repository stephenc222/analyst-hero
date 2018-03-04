#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define CENTER_WIDTH SCREEN_WIDTH / 2
#define CENTER_HEIGHT  SCREEN_HEIGHT / 2
#define TRUE 1
#define FALSE 0
SDL_Color WHITE = {255, 255, 255};
SDL_Color BLACK = {0, 0, 0};

typedef struct {
  SDL_Rect rect;
  int fontSize;
  TTF_Font* font;
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
  int numSceneRects;
  TextRect* currentScene;
  Player* player;
  Boss* firstBoss;
  Boss* finalBoss;
  NPC* maleOne;
  NPC* femaleOne;
  NPC* femaleTwo;
  Inventory* inventory;
} Game;

// Forward Declarations
Game* newGame(char* text);
void destroyGame(Game* game);
Inventory* newInventory();
void destroyInventory(Inventory* inventory);
Player* newPlayer();
void destroyPlayer(Player* player);
Boss* newBoss();
void destroyBoss(Boss* boss);
NPC* newNPC();
void destroyNPC(NPC* npc);
TextRect* newCurrentScene(int numSceneRects);
void destroyCurrentScene(TextRect* textRect, int numSceneRects);
TextRect* newTextRect(int x, int y, int w, int h, int fontSize ,char* filename, char* text);
void destroyTextRect(TextRect* textRect);

int initGame(void);

void loadTitleScene(Game* game);
void loadPlayScene(Game* game, SDL_Rect titleSceneRects[]);
void loadFightScene(Game* game, SDL_Rect titleSceneRects[]);
void loadGameoverScene(Game* game, SDL_Rect titleSceneRects[]);
void loadGameWinScene(Game* game, SDL_Rect titleSceneRects[]);

void cleanUp(SDL_Renderer *renderer,SDL_Window *window, Game* game);
TTF_Font* loadText(const char *fileName, int fontSize);
void renderText(SDL_Renderer *renderer,TTF_Font *font,SDL_Rect rect, char *text);
void render(SDL_Renderer *renderer, Game* game);
void renderScene(SDL_Renderer *renderer,Game* game);
void updateScene(SDL_Renderer *renderer, Game* game);

// function definitions
void destroyGame(Game* game) {
  if (game) {
    destroyPlayer(game->player);
    destroyInventory(game->inventory);
    destroyBoss(game->firstBoss);
    destroyBoss(game->finalBoss);
    destroyNPC(game->maleOne);
    destroyNPC(game->femaleOne);
    destroyNPC(game->femaleTwo);
    destroyCurrentScene(game->currentScene, game->numSceneRects);
    free(game);
    game = 0;
  }
}

Inventory* newInventory() {
  Inventory* inventory = malloc(sizeof *inventory);
  return inventory;
}

void destroyInventory(Inventory* inventory) {
  if (inventory) {
    free(inventory);
    inventory = 0;
  }
}

Player* newPlayer() {
  Player* player = malloc(sizeof *player);
  return player;
}

void destroyPlayer(Player* player) {
  if (player) {
    free(player);
    player = 0;
  }
}

Boss* newBoss() {
  Boss* boss = malloc(sizeof *boss);
  return boss;
}

void destroyBoss(Boss* boss) {
  if (boss) {
    free(boss);
    boss = 0;
  }
}

NPC* newNPC() {
  NPC* npc = malloc(sizeof *npc);
  return npc;
}

void destroyNPC(NPC* npc) {
  if (npc) {
    free(npc);
    npc = 0;
  }
}

TextRect* newCurrentScene(int numSceneRects) {
  TextRect* currentScene = malloc(numSceneRects * sizeof(TextRect));
  return currentScene;
}

void destroyCurrentScene(TextRect* currentScene, int numSceneRects) {
  if (currentScene) {
    for (int i = 0; i < numSceneRects; ++i) {
      if (currentScene[i].font) {
        TTF_CloseFont(currentScene[i].font);
      }
    }
    free(currentScene);
    currentScene = 0;
  }
}

TextRect* newTextRect(int x, int y, int w, int h, int fontSize ,char* filename, char* text) {
  TextRect* textRect = malloc(sizeof *textRect);

  textRect->rect.x  = x;
  textRect->rect.y = y;
  textRect->rect.w = w;
  textRect->rect.h = h;
  textRect->text = text;
  textRect->font = loadText(filename, fontSize);
  return textRect;
}

void destroyTextRect(TextRect* textRect) {
  if (textRect) {
    TTF_CloseFont(textRect->font);
    free(textRect);
    textRect = 0;
  }
}

void loadTitleScene(Game* game) {
    // title scene
  SDL_Rect titleRect;
  SDL_Rect subTitleRect;
  SDL_Rect startGameRect;

  // char* titleText =  "Analyst Hero";
  // char* subText =  "Click to start game";
  char* startText =  "START GAME";

  TextRect* titleTextRect = newTextRect(
    CENTER_WIDTH - 100,
    CENTER_HEIGHT - 150,
    200,
    100, 
    100,
    "../assets/OpenSans-Bold.ttf",
    "Analyst Hero"
  );

  TextRect* subTitleTextRect = newTextRect(
    CENTER_WIDTH - 100,
    CENTER_HEIGHT - 50,
    200,
    60, 
    40,
    "../assets/OpenSans-Bold.ttf",
    "Click to start game"
  );

  TextRect* startGameTextRect = newTextRect(
    CENTER_WIDTH - 100,
    CENTER_HEIGHT,
    200,
    70, 
    70,
    "../assets/OpenSans-Bold.ttf",
    "START GAME"
  );

  TextRect* titleSceneRects[3] = {titleTextRect, subTitleTextRect, startGameTextRect};
  if (game->currentScene) {
    destroyCurrentScene(game->currentScene, game->numSceneRects);
  }
  game->numSceneRects = 3;
  game->currentScene = newCurrentScene(3);

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

  game->player = newPlayer();
  game->inventory = newInventory();
  game->firstBoss = newBoss();
  game->finalBoss = newBoss();
  game->maleOne = newNPC();
  game->femaleOne = newNPC();
  game->femaleTwo = newNPC();
  game->currentScene  = 0;

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

void cleanUp(SDL_Renderer *renderer,SDL_Window *window, Game* game) {
  // FIXME: probably need to include destroying a texture here...
  // SDL_DestroyTexture(texture);
  destroyGame(game);
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
        // free(game);
        cleanUp(renderer, window, game);
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