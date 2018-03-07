#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SPRITE_WIDTH 64
#define SPRITE_HEIGHT 64
#define COMPUTER_WIDTH 55
#define COMPUTER_HEIGHT 50
#define TILE_HEIGHT 20
#define TILE_WIDTH 20
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
  SDL_Texture* texture;
  char* text;
  int isShown;
} TextRect;

typedef struct {
  int reportAmmo;
  int coffee;
} Inventory;

typedef struct {
  int health;
  int status; // TODO: enum values for different statuses
  int timeScore;
  int reportEff;
  SDL_Texture* texture;  
  SDL_Rect srcRect;
  SDL_Rect destRect;
  int x;
  int y;
} Player;

typedef struct {
  int health;
  int status;
  char* name;
  SDL_Texture* texture;  
  // NOTE: srcRect relates to the texture, not any game location
  SDL_Rect srcRect;
  SDL_Rect destRect;
  int x;
  int y;
} Boss;

typedef struct {
  char* itemName;
  SDL_Texture* texture;  
  SDL_Rect srcRect;
  SDL_Rect destRect;
  int x;
  int y;
} Item;

typedef struct {
  int startX, startY;
  int endX, endY;
  SDL_Texture* texture;
  SDL_Rect srcRect;
  SDL_Rect destRect;
  int x;
  int y;
} NPC;

typedef struct {
  SDL_Texture* wallTexture;
  SDL_Texture* groundTexture;
  int textureMapArr;
} GameMap;

typedef struct {
  int isKeyDown;
  int isMouseDown;
  double mouseX;
  double mouseY;
  int whichKey;
} GameInput;

typedef struct {
  int numSceneRects;
  GameMap* gameMap;
  GameInput* gameInput;
  TextRect* currentScene;
  Player* player;
  Boss* firstBoss;
  Boss* finalBoss;
  NPC* maleOne;
  NPC* femaleOne;
  NPC* femaleTwo;
  Inventory* inventory;
  // TODO: add waterCooler and copier Items :)
  Item* computer;
  char* currentSceneName;
} Game;

// Forward Declarations
Game* newGame(SDL_Renderer* renderer);
void destroyGame(Game* game);
GameInput* newGameInput();
void destroyGameInput(GameInput* gameInput);
GameMap* newGameMap(SDL_Renderer* renderer);
void destroyGameMap(GameMap* gameMap);
Inventory* newInventory();
void destroyInventory(Inventory* inventory);
Item* newItem(SDL_Renderer* renderer, char* itemName, char* filename, int width, int height);
void destroyItem(Item* item);
Player* newPlayer(SDL_Renderer* renderer);
void destroyPlayer(Player* player);
Boss* newBoss(SDL_Renderer* renderer, char* filename);
void destroyBoss(Boss* boss);
NPC* newNPC(SDL_Renderer* renderer, char* filename);
void destroyNPC(NPC* npc);
TextRect* newCurrentScene(int numSceneRects);
void destroyCurrentScene(TextRect* textRect, int numSceneRects);
TextRect* newTextRect(SDL_Renderer* renderer, int x, int y, int w, int h, int fontSize ,char* filename, char* text, int isShown);
void destroyTextRect(TextRect* textRect);

int initGame(void);
void handleInput(Game* game, int *loop);
void update(SDL_Renderer* renderer,Game* game, float dt);

void updateTitleScene(SDL_Renderer* renderer, Game* game, float dt);
void updatePlayScene(SDL_Renderer* renderer, Game* game, float dt);
void updateGameoverScene(SDL_Renderer* renderer, Game* game, float dt);
void updateGameoverScene(SDL_Renderer* renderer, Game* game, float dt);
void updateGameWinScene(SDL_Renderer* renderer, Game* game, float dt);

void loadTitleScene(SDL_Renderer* renderer, Game* game);
void loadPlayScene(SDL_Renderer* renderer, Game* game);
void loadFightScene(SDL_Renderer* renderer,Game* game);
void loadGameoverScene(SDL_Renderer* renderer, Game* game);
void loadGameWinScene(SDL_Renderer* renderer,Game* game);

void cleanUp(SDL_Renderer *renderer,SDL_Window *window, Game* game);
TTF_Font* loadText(const char *fileName, int fontSize);
void renderText(SDL_Renderer *renderer, SDL_Texture *texture,TTF_Font *font,SDL_Rect rect, char *text);
void render(SDL_Renderer *renderer, Game* game);
void renderMap(SDL_Renderer * renderer, Game* game);
void renderMapTile(SDL_Renderer *renderer, Game* game, SDL_Texture* texture, SDL_Rect* destRect);
void renderPlayer(SDL_Renderer *renderer, Game* game);
void renderNPC(SDL_Renderer *renderer, Game* game, NPC* npc);
void renderBoss(SDL_Renderer *renderer, Game* game, Boss* boss);
void renderScene(SDL_Renderer *renderer,Game* game);
void renderPlayScene(SDL_Renderer *renderer, Game* game);
void updateScene(SDL_Renderer *renderer, Game* game);
void handleWhichKey(Game* game, SDL_Keysym *keysym);

void updateTitleScene(SDL_Renderer* renderer,Game* game, float dt) {
  // printf("UPDATE TITLE SCENE: %s\n", game->currentSceneName);  
  if (game->gameInput->whichKey == SDL_SCANCODE_RETURN) {
    loadPlayScene(renderer,game);
  }
}

void updatePlayScene(SDL_Renderer* renderer,Game* game, float dt) {
  // printf("UPDATE PLAY SCENE: %s\n", game->currentSceneName);  
}

void update(SDL_Renderer* renderer, Game* game, float dt) { 
  // TODO: implement game update logic
  // printf("CURRENT SCENE: %s\n", game->currentSceneName);
  // fflush(stdout);
  if (strncmp(game->currentSceneName, "title", 4) == 0) {
    updateTitleScene(renderer,game, dt);
  } else if (strncmp(game->currentSceneName, "play", 4) == 0) {
    updatePlayScene(renderer, game, dt);
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

GameMap* newGameMap(SDL_Renderer* renderer) {
  GameMap* gameMap = malloc(sizeof *gameMap);
  SDL_Surface* wall = IMG_Load("../assets/color-wall-6b5445.png");
  SDL_Surface* ground = IMG_Load("../assets/color-floor-bedcb0.png");

  SDL_Texture* wallTexture = SDL_CreateTextureFromSurface(renderer, wall);
  SDL_Texture* groundTexture = SDL_CreateTextureFromSurface(renderer, ground);

  SDL_FreeSurface(wall);
  SDL_FreeSurface(ground);

  gameMap->groundTexture = groundTexture;
  gameMap->wallTexture = wallTexture;

  // TODO: add an int-based mapping array for rendering either a ground or wall texture
  return gameMap;
}

void destroyGameMap(GameMap* gameMap) {
  if (gameMap) {
    if (gameMap->groundTexture) {
      free(gameMap->groundTexture);
    }
    if (gameMap->wallTexture) {
      free(gameMap->wallTexture);
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
    destroyItem(game->computer);
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

Player* newPlayer(SDL_Renderer* renderer) {
  Player* player = malloc(sizeof *player);

  SDL_Surface* image = IMG_Load("../assets/main_character.png");
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
  SDL_Rect srcRect;
  SDL_Rect destRect;
  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.w = SPRITE_WIDTH;
  srcRect.h = SPRITE_HEIGHT;
  destRect.x = 0;
  destRect.y = 0;
  destRect.w = SPRITE_WIDTH;
  destRect.h = SPRITE_HEIGHT;

  player->srcRect = srcRect;
  player->destRect = destRect;
  player->x = 0;
  player->y = 0;
  player->health = 100;
  player->timeScore = 0;
  player->reportEff = 1;
  player->status = 1;
  player->texture = texture;

  return player;
}

void destroyPlayer(Player* player) {
  if (player) {
    if (player->texture) {
      SDL_DestroyTexture(player->texture);
    }
    free(player);
    player = 0;
  }
}

Boss* newBoss(SDL_Renderer* renderer, char* filename) {
  Boss* boss = malloc(sizeof *boss);
  SDL_Surface* image = IMG_Load(filename);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);

  SDL_Rect srcRect;
  SDL_Rect destRect;
  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.w = SPRITE_WIDTH;
  srcRect.h = SPRITE_HEIGHT;
  destRect.x = 0;
  destRect.y = 0;
  destRect.w = SPRITE_WIDTH;
  destRect.h = SPRITE_HEIGHT;

  boss->srcRect = srcRect;
  boss->destRect = destRect;
  boss->x = 0;
  boss->y = 0;
  boss->health = 100;
  boss->status = 1;
  boss->texture = texture;

  return boss;
}

void destroyBoss(Boss* boss) {
  if (boss) {
    if (boss->texture) {
      SDL_DestroyTexture(boss->texture);
    }
    free(boss);
    boss = 0;
  }
}

NPC* newNPC(SDL_Renderer* renderer, char* filename) {
  NPC* npc = malloc(sizeof *npc);
  SDL_Surface* image = IMG_Load(filename);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
  
  SDL_Rect srcRect;
  SDL_Rect destRect;
  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.w = SPRITE_WIDTH;
  srcRect.h = SPRITE_HEIGHT;
  destRect.x = 0;
  destRect.y = 0;
  destRect.w = SPRITE_WIDTH;
  destRect.h = SPRITE_HEIGHT;

  npc->srcRect = srcRect;
  npc->destRect = destRect;
  npc->x = 0;
  npc->y = 0;
  npc->texture = texture;

  return npc;
}

void destroyNPC(NPC* npc) {
  if (npc) {
    if (npc->texture) {
      SDL_DestroyTexture(npc->texture);
    }
    free(npc);
    npc = 0;
  }
}

Item* newItem(SDL_Renderer* renderer, char* itemName, char* filename, int width, int height) {
  Item* item = malloc(sizeof *item);
  SDL_Surface* image = IMG_Load(filename);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
  
  SDL_Rect srcRect;
  SDL_Rect destRect;
  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.w = width;
  srcRect.h = height;
  destRect.x = 0;
  destRect.y = 0;
  destRect.w = width;
  destRect.h = height;

  item->itemName = itemName;
  item->srcRect = srcRect;
  item->destRect = destRect;
  item->x = 0;
  item->y = 0;
  item->texture = texture;

  return item;
}

void destroyItem(Item* item) {
  if (item) {
    if (item->texture) {
      SDL_DestroyTexture(item->texture);
    }
    free(item);
    item = 0;
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
      if (currentScene[i].texture) {
        SDL_DestroyTexture(currentScene[i].texture);
      }
    }
    free(currentScene);
    currentScene = 0;
  }
}

TextRect* newTextRect(SDL_Renderer *renderer,int x, int y, int w, int h, int fontSize ,char* filename, char* text, int isShown) {
  TextRect* textRect = malloc(sizeof *textRect);

  textRect->font = loadText(filename, fontSize);

  SDL_Surface* surface = TTF_RenderText_Solid(textRect->font, text, BLACK);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  textRect->texture = texture;
  textRect->rect.x  = x;
  textRect->rect.y = y;
  textRect->rect.w = w;
  textRect->rect.h = h;
  textRect->text = text;
  textRect->isShown = isShown;
  return textRect;
}

void destroyTextRect(TextRect* textRect) {
  if (textRect) {
    TTF_CloseFont(textRect->font);
    free(textRect);
    textRect = 0;
    if (textRect->texture) {
      SDL_DestroyTexture(textRect->texture);
    }
  }
}

void loadTitleScene(SDL_Renderer* renderer,Game* game) {

  TextRect* titleTextRect = newTextRect(
    renderer,
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
    renderer,
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
    renderer,
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

void loadPlayScene(SDL_Renderer* renderer,Game* game) {
  // TODO: main play scene
  TextRect* playingGameTextRect = newTextRect(
    renderer,
    CENTER_WIDTH - 100,
    CENTER_HEIGHT,
    200,
    70, 
    70,
    "../assets/OpenSans-Bold.ttf",
    "-PLAYING GAME-",
    1
  );
  
  if (game->currentScene) {
    destroyCurrentScene(game->currentScene, game->numSceneRects);
  }

  game->currentSceneName = "play";
  TextRect* playSceneRects[1] = {playingGameTextRect};

  game->numSceneRects = 1;
  game->currentScene = newCurrentScene(1);

  for (int i = 0; i < 1; ++i) {
    game->currentScene[i] = *playSceneRects[i];
    printf("test again: %s \n", game->currentScene[i].text);
  }
}

void loadFightScene(SDL_Renderer* renderer, Game* game) {
  // TODO: mini game fight scene
}

void loadGameoverScene(SDL_Renderer* renderer, Game* game) {
  // TODO: Gameover scene
}

void loadGameWinScene(SDL_Renderer* renderer, Game* game) {
  // TODO: GameWin scene
}

// Definitions
void renderText(SDL_Renderer *renderer, SDL_Texture *texture, TTF_Font *font,SDL_Rect rect, char *text) {
  SDL_RenderCopy(renderer, texture, NULL, &rect); 
}

void renderMap(SDL_Renderer * renderer, Game* game) {
  // renderMapTile(renderer, game);
  // TODO: for all local SDL_Rect types in specific render functions: remove and reference
  // the object on the game object instead
  SDL_Rect wallDestRect;
  wallDestRect.x = 40;
  wallDestRect.y = 40;
  wallDestRect.w = TILE_WIDTH;
  wallDestRect.h = TILE_HEIGHT;
  renderMapTile(renderer, game, game->gameMap->wallTexture, &wallDestRect);

  SDL_Rect groundDestRect;
  groundDestRect.x = 80;
  groundDestRect.y = 40;
  groundDestRect.w = TILE_WIDTH;
  groundDestRect.h = TILE_HEIGHT;
  renderMapTile(renderer, game, game->gameMap->groundTexture, &groundDestRect);
}

void renderMapTile(SDL_Renderer *renderer, Game* game, SDL_Texture* texture, SDL_Rect* destRect) {
  SDL_RenderCopy(renderer, texture, NULL, destRect); 
}

void renderPlayer(SDL_Renderer *renderer, Game* game) {
  SDL_Rect playerSrcRect;
  playerSrcRect.x = 0;
  playerSrcRect.y = 0;
  playerSrcRect.w = SPRITE_WIDTH;
  playerSrcRect.h = SPRITE_HEIGHT;

  SDL_Rect playerDestRect;
  playerDestRect.x = 340;
  playerDestRect.y = 40;
  playerDestRect.w = SPRITE_WIDTH;
  playerDestRect.h = SPRITE_HEIGHT;
  SDL_RenderCopyEx(renderer, game->player->texture, &playerSrcRect, &playerDestRect, 0, NULL, SDL_FLIP_NONE); 
}

void renderNPC(SDL_Renderer *renderer, Game* game, NPC* npc) {
  SDL_Rect npcSrcRect;
  npcSrcRect.x = 0;
  npcSrcRect.y = 0;
  npcSrcRect.w = SPRITE_WIDTH;
  npcSrcRect.h = SPRITE_HEIGHT;

  SDL_Rect npcDestRect;
  npcDestRect.x = 240;
  npcDestRect.y = 80;
  npcDestRect.w = SPRITE_WIDTH;
  npcDestRect.h = SPRITE_HEIGHT;
  SDL_RenderCopyEx(renderer, npc->texture, &npcSrcRect, &npcDestRect, 0, NULL, SDL_FLIP_NONE); 

  // SDL_RenderCopyEx(renderer, texture, NULL, &rect, 0, NULL, SDL_FLIP_NONE); 
}

void renderItem(SDL_Renderer *renderer, Game* game, Item* item) {
  SDL_Rect itemSrcRect;
  itemSrcRect.x = 0;
  itemSrcRect.y = 0;
  itemSrcRect.w = COMPUTER_WIDTH;
  itemSrcRect.h = COMPUTER_HEIGHT;

  SDL_Rect itemDestRect;
  itemDestRect.x = 240;
  itemDestRect.y = 80;
  itemDestRect.w = COMPUTER_WIDTH;
  itemDestRect.h = COMPUTER_HEIGHT;
  SDL_RenderCopyEx(renderer, item->texture, &itemSrcRect, &itemDestRect, 0, NULL, SDL_FLIP_NONE); 

}

void renderBoss(SDL_Renderer *renderer, Game* game, Boss* boss) {
  SDL_Rect bossSrcRect;
  bossSrcRect.x = 0;
  bossSrcRect.y = 0;
  bossSrcRect.w = SPRITE_WIDTH;
  bossSrcRect.h = SPRITE_HEIGHT;

  SDL_Rect bossDestRect;
  bossDestRect.x = 140;
  bossDestRect.y = 80;
  bossDestRect.w = SPRITE_WIDTH;
  bossDestRect.h = SPRITE_HEIGHT;
  SDL_RenderCopyEx(renderer, boss->texture, &bossSrcRect, &bossDestRect, 0, NULL, SDL_FLIP_NONE); 

  // SDL_RenderCopyEx(renderer, texture, NULL, &rect, 0, NULL, SDL_FLIP_NONE); 
}

void renderPlayScene(SDL_Renderer *renderer, Game* game) {
  // renders:
  // 1. Game Environment - TODO: in progress
  // 2. Player - TODO: in progress
  // 3. NPCs - TODO: in progress
  // 4. Bosses - TODO: in progress
  
  renderMap(renderer, game);
  renderPlayer(renderer, game);
  renderItem(renderer,game, game->computer);
  renderNPC(renderer, game, game->maleOne);
  renderNPC(renderer, game, game->femaleOne);
  renderNPC(renderer, game, game->femaleTwo);
  renderBoss(renderer, game, game->firstBoss);
  renderBoss(renderer, game, game->finalBoss);


}

void renderScene(SDL_Renderer *renderer, Game* game) {
  // render scene specific stuff here
  if (strncmp(game->currentSceneName, "play", 4) == 0) {
    renderPlayScene(renderer, game);
  }

  // renders text of scene - assuming every scene has text
  for (int i = 0; i < game->numSceneRects; ++i) {
    if (game->currentScene[i].isShown) {
      renderText(
        renderer, 
        game->currentScene[i].texture,
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

Game* newGame(SDL_Renderer* renderer) {
  // TTF_Font* font = TTF_OpenFont(fileName, 24); //this opens a font style and sets a size
  // TODO: maybe add error handling here?

  // *note to self = must allocate (malloc) memory for structs
  Game* game = malloc(sizeof *game);

  game->gameInput = newGameInput();
  game->gameMap = newGameMap(renderer);
  game->player = newPlayer(renderer);
  game->inventory = newInventory();
  game->computer = newItem(renderer, "computer", "../assets/workstation-computer-55x50.png", COMPUTER_WIDTH, COMPUTER_HEIGHT);
  game->firstBoss = newBoss(renderer, "../assets/first_boss.png");
  game->finalBoss = newBoss(renderer, "../assets/final_boss.png");
  game->maleOne = newNPC(renderer, "../assets/male_1.png");
  game->femaleOne = newNPC(renderer, "../assets/female_1.png");
  game->femaleTwo = newNPC(renderer, "../assets/female_1.png");
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
  IMG_Quit();
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

  // for allowing loading of PNG files. IMG_INIT_PNG == 2
  if(IMG_Init(IMG_INIT_PNG) != 2){
    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    return 1;
  }   

  // TODO: refactor game struct initializer
  Game* game = newGame(renderer);
  loadTitleScene(renderer, game);

  int loop = 1;

  float dt = 0;
  double lastTime = 0, currentTime;

  while(loop) {
    handleInput(game, &loop);

    currentTime = SDL_GetTicks();
    if ((currentTime - lastTime) < 1000) {
      dt = (currentTime - lastTime);
      update(renderer,game, dt);
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