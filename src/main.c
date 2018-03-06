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
  int isShown;
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

// TODO: add struct for game user input handling
typedef struct {
  int isKeyDown;
  int isMouseDown;
  double mouseX;
  double mouseY;
  int whichKey;
} GameInput;

typedef struct {
  int numSceneRects;
  GameInput* gameInput;
  TextRect* currentScene;
  Player* player;
  Boss* firstBoss;
  Boss* finalBoss;
  NPC* maleOne;
  NPC* femaleOne;
  NPC* femaleTwo;
  Inventory* inventory;
  char* currentSceneName;
} Game;

// Forward Declarations
Game* newGame(char* text);
void destroyGame(Game* game);
GameInput* newGameInput();
void destroyGameInput(GameInput* gameInput);
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
TextRect* newTextRect(int x, int y, int w, int h, int fontSize ,char* filename, char* text, int isShown);
void destroyTextRect(TextRect* textRect);

int initGame(void);
void handleInput(Game* game, int *loop);
void update(Game* game, float dt);

void updateTitleScene(Game* game, float dt);
void updatePlayScene(Game* game, float dt);
void updateGameoverScene(Game* game, float dt);
void updateGameoverScene(Game* game, float dt);
void updateGameWinScene(Game* game, float dt);

void loadTitleScene(Game* game);
void loadPlayScene(Game* game);
void loadFightScene(Game* game);
void loadGameoverScene(Game* game);
void loadGameWinScene(Game* game);

void cleanUp(SDL_Renderer *renderer,SDL_Window *window, Game* game);
TTF_Font* loadText(const char *fileName, int fontSize);
void renderText(SDL_Renderer *renderer,TTF_Font *font,SDL_Rect rect, char *text);
void render(SDL_Renderer *renderer, Game* game);
void renderScene(SDL_Renderer *renderer,Game* game);
void updateScene(SDL_Renderer *renderer, Game* game);
void handleWhichKey(Game* game, SDL_Keysym *keysym);

void updateTitleScene(Game* game, float dt) {
  // printf("UPDATE TITLE SCENE: %s\n", game->currentSceneName);  
  if (game->gameInput->whichKey == SDL_SCANCODE_RETURN) {
    loadPlayScene(game);
  }
}

void updatePlayScene(Game* game, float dt) {
  // printf("UPDATE PLAY SCENE: %s\n", game->currentSceneName);  
}

void update(Game* game, float dt) { 
  // TODO: implement game update logic
  // printf("CURRENT SCENE: %s\n", game->currentSceneName);
  // fflush(stdout);
  if (strncmp(game->currentSceneName, "title", 4) == 0) {
    updateTitleScene(game, dt);
  } else if (strncmp(game->currentSceneName, "play", 4) == 0) {
    updatePlayScene(game, dt);
  }
}

void handleWhichKey(Game* game, SDL_Keysym *keysym) {
  switch (keysym->scancode) {
    case SDL_SCANCODE_A: {
      // printf("A pressed!\n");
      game->gameInput->whichKey = SDL_SCANCODE_LEFT;
      break;
    }
    case SDL_SCANCODE_LEFT: {
      // printf("left pressed!\n");
      game->gameInput->whichKey = SDL_SCANCODE_LEFT;
      break;
    }
    case SDL_SCANCODE_D: {
      // printf("D pressed!\n");
      game->gameInput->whichKey = SDL_SCANCODE_RIGHT;
      break;
    }
    case SDL_SCANCODE_RIGHT: {
      // printf("right pressed!\n");
      game->gameInput->whichKey = SDL_SCANCODE_RIGHT;
      break;
    }
    case SDL_SCANCODE_W: {
      // printf("W pressed!\n");
      game->gameInput->whichKey = SDL_SCANCODE_UP;
      break;
    }
    case SDL_SCANCODE_UP: {
      // printf("up pressed!\n");
      game->gameInput->whichKey = SDL_SCANCODE_UP;
      break;
    }
    case SDL_SCANCODE_S: {
      // printf("S pressed!\n");
      game->gameInput->whichKey = SDL_SCANCODE_DOWN;
      break;
    }
    case SDL_SCANCODE_DOWN: {
      // printf("down pressed!\n");
      game->gameInput->whichKey = SDL_SCANCODE_DOWN;
      break;
    }
    case SDL_SCANCODE_RETURN: {
      // printf("Enter pressed!\n");
      game->gameInput->whichKey = SDL_SCANCODE_RETURN;
      break;
    }
    default: {
      break;
    }
  }
}

// function definitions
void handleInput(Game* game, int *loop) {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    // handle user events
    // for debugging the various event type enum values
    // printf("event.type: %d \n", event.type);
    switch (event.type) {
      case SDL_QUIT: {
        // exit the outer while loop controlled by this loop variable
        *loop = 0;        
        break;
      }
      case SDL_KEYDOWN: {
        // printf("keyDown and: %u\n", event.key.keysym);
        if (game->gameInput->isKeyDown == 1) {
          break;
        }
        game->gameInput->isKeyDown = 1;
        handleWhichKey(game, &event.key.keysym);
        break;
      }
      case SDL_KEYUP: {
        // printf("keyUp!\n");
        game->gameInput->isKeyDown = 0;        
        break;
      }
      case SDL_MOUSEBUTTONDOWN: {
        // printf("mouseDown!\n");
        game->gameInput->isMouseDown = 1;                
        break;
      }
      case SDL_MOUSEBUTTONUP: {
        // printf("mouseUp!\n");
        game->gameInput->isMouseDown = 0;                
        break;
      }
      case SDL_MOUSEMOTION: {
        // printf("X: %d, Y: %d \n", event.motion.x, event.motion.y);
        game->gameInput->mouseX = event.motion.x;
        game->gameInput->mouseY = event.motion.y;
        break;
      }
      default: {
        break;
      }
    }
  }
}

GameInput* newGameInput() {
  
  GameInput* gameInput = malloc(sizeof *gameInput);
  gameInput->isKeyDown = 0;
  gameInput->mouseX = 0;
  gameInput->mouseY = 0;
  gameInput->whichKey = 0;
  gameInput->isMouseDown = 0;

  return gameInput;
}

void destroyGameInput(GameInput* gameInput) {
  if (gameInput) {
    free(gameInput);
  }
}

void destroyGame(Game* game) {
  if (game) {
    destroyPlayer(game->player);
    destroyGameInput(game->gameInput);
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

TextRect* newTextRect(int x, int y, int w, int h, int fontSize ,char* filename, char* text, int isShown) {
  TextRect* textRect = malloc(sizeof *textRect);

  textRect->rect.x  = x;
  textRect->rect.y = y;
  textRect->rect.w = w;
  textRect->rect.h = h;
  textRect->text = text;
  textRect->isShown = isShown;
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

  TextRect* titleTextRect = newTextRect(
    CENTER_WIDTH - 100,
    CENTER_HEIGHT - 150,
    200,
    100, 
    100,
    "../assets/OpenSans-Bold.ttf",
    "Analyst Hero",
    1
  );

  TextRect* subTitleTextRect = newTextRect(
    CENTER_WIDTH - 100,
    CENTER_HEIGHT - 50,
    200,
    60, 
    40,
    "../assets/OpenSans-Bold.ttf",
    "Press Enter to start game",
    1
  );

  TextRect* startGameTextRect = newTextRect(
    CENTER_WIDTH - 100,
    CENTER_HEIGHT,
    200,
    70, 
    70,
    "../assets/OpenSans-Bold.ttf",
    "START GAME",
    1
  );

  game->currentSceneName = "title";

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

void loadPlayScene(Game* game) {
  // TODO: main play scene
  TextRect* playingGameTextRect = newTextRect(
    CENTER_WIDTH - 100,
    CENTER_HEIGHT,
    200,
    70, 
    70,
    "../assets/OpenSans-Bold.ttf",
    "-PLAYING GAME-",
    1
  );
  
  game->currentSceneName = "play";
  TextRect* playSceneRects[1] = {playingGameTextRect};
  
  if (game->currentScene) {
    destroyCurrentScene(game->currentScene, game->numSceneRects);
  }

  game->numSceneRects = 1;
  game->currentScene = newCurrentScene(1);

  for (int i = 0; i < 1; ++i) {
    game->currentScene[i] = *playSceneRects[i];
    printf("test again: %s \n", game->currentScene[i].text);
  }
}

void loadFightScene(Game* game) {
  // TODO: fight scene
}

void loadGameoverScene(Game* game) {
  // TODO: Gameover scene
}

void loadGameWinScene(Game* game) {
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

void renderScene(SDL_Renderer *renderer, Game* game) {
  // render scene specific stuff here
  for (int i = 0; i < game->numSceneRects; ++i) {
    if (game->currentScene[i].isShown) {
      renderText(
        renderer, 
        game->currentScene[i].font,  
        game->currentScene[i].rect, 
        game->currentScene[i].text
      );
    }
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

  game->gameInput = newGameInput();
  game->player = newPlayer();
  game->inventory = newInventory();
  game->firstBoss = newBoss();
  game->finalBoss = newBoss();
  game->maleOne = newNPC();
  game->femaleOne = newNPC();
  game->femaleTwo = newNPC();
  game->currentScene = 0;

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

  if(TTF_Init() != 0) {
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

  int loop = 1;

  float dt = 0;
  double lastTime = 0, currentTime;

  while(loop) {
    handleInput(game, &loop);

    currentTime = SDL_GetTicks();
    if ((currentTime - lastTime) < 1000) {
      dt = (currentTime - lastTime);
      update(game, dt);
    }
    lastTime = currentTime;
    render(renderer, game);
  }
  cleanUp(renderer, window, game);
  return 0;
};

int main(int argc, char *argv[]) {
  initGame();
};