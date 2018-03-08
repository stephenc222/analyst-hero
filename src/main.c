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
#define COPIER_WIDTH 55
#define COPIER_HEIGHT 55
#define WATERCOOLER_WIDTH 40
#define WATERCOOLER_HEIGHT 60
#define TILE_HEIGHT 20
#define TILE_WIDTH 20
#define CENTER_WIDTH SCREEN_WIDTH / 2
#define CENTER_HEIGHT  SCREEN_HEIGHT / 2
#define TRUE 1
#define FALSE 0
#define MAP_W 32
#define MAP_H 24
#define SHOW_TEXT 1
#define HIDE_TEXT 0

SDL_Color WHITE = {255, 255, 255};
SDL_Color BLACK = {0, 0, 0};

unsigned char gameMapArr[MAP_W * MAP_H] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

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
  float time;
  float rate;
  int frame;
  int numFrames;
  int startFrame;
  int animation;
  int frameHeight;
  int frameWidth;
  unsigned char loop;
  unsigned char hflip;
} Sprite;

typedef struct {
  int health;
  int status; // TODO: enum values for different statuses
  int timeScore;
  int reportEff;
  SDL_Texture* texture;  
  SDL_Rect srcRect;
  SDL_Rect destRect;
  Sprite* sprite;
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
  Sprite* sprite;
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
  Sprite* sprite;  
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

typedef struct t_Game {
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
  Item* waterCooler;
  Item* copier;
  char* currentSceneName;
  void (*updateFunc)(SDL_Renderer* renderer, struct t_Game* game, float dt);
  void (*renderFunc)(SDL_Renderer* renderer, struct t_Game* game);
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
Sprite* newSprite();
void destroySprite(Sprite* sprite);
TextRect* newTextRect(SDL_Renderer* renderer, int x, int y, int w, int h, int fontSize ,char* filename, char* text, int isShown, SDL_Color color);
void destroyTextRect(TextRect* textRect);

int initGame(void);
int getPlayerCollision(Game* game, Player* player);
void handleInput(Game* game, int *loop);
void update(SDL_Renderer* renderer,Game* game, float dt);

void updateTitleScene(SDL_Renderer* renderer, Game* game, float dt);
void updatePlayScene(SDL_Renderer* renderer, Game* game, float dt);
void updateSprite(Sprite* sprite, float dt);
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
void renderPlayer(SDL_Renderer *renderer, Game* game, Player* player);
void renderNPC(SDL_Renderer *renderer, Game* game, NPC* npc);
void renderBoss(SDL_Renderer *renderer, Game* game, Boss* boss);
void renderScene(SDL_Renderer *renderer,Game* game);
void renderPlayScene(SDL_Renderer *renderer, Game* game);
void renderTitleScene(SDL_Renderer *renderer, Game* game);
void handleWhichKey(Game* game, SDL_Keysym *keysym);

void updateTitleScene(SDL_Renderer* renderer,Game* game, float dt) {
  // printf("UPDATE TITLE SCENE: %s\n", game->currentSceneName);  
  if (game->gameInput->whichKey == SDL_SCANCODE_RETURN) {
    SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
    loadPlayScene(renderer,game);
  }
}

void updateSprite(Sprite* sprite, float dt) {
  // add time to sprite time accumulator
	sprite->time += dt;
	// if the accumulated sprite time reaches the sprite frame rate (time to show each frame)
	if (sprite->time >= sprite->rate) {
		// reset the sprite time accumulator
		sprite->time -= sprite->rate;
		// increment sprite frame
		sprite->frame += 1;
		// if the sprite frame is beyond the number of frames
		if (sprite->frame >= sprite->numFrames) {
			// if the sprite is set to loop animation
			if (sprite->loop) {
				// go back to first frame
				sprite->frame = sprite->startFrame;
			} else {
				// back up to the last sprite frame
				sprite->frame -= 1;
			}
		}
	}
}

void renderPlayer(SDL_Renderer* renderer,Game* game, Player* player) {
  // set the source rect coords
  // animation #8 -> walking up
  // animation #9 -> walking right
  // animation #10 -> walking down
  // animation #11 -> walking left
	player->srcRect.x = player->sprite->frame * player->sprite->frameWidth;
	player->srcRect.y = player->sprite->animation * player->sprite->frameHeight;

  // set the destination rect coords
	player->destRect.x = (int)player->x;
	player->destRect.y = (int)player->y;
	
  // blast the player->sprite frame to the renderer target
	SDL_RenderCopyEx(
	  renderer,
    player->texture,
    &player->srcRect,
    &player->destRect,
    0, // no rotation
    0, // no rotation center point (uses center of sprite if rotation above has a value)
    player->sprite->hflip // not flipped
  );
  SDL_RenderDrawRect(renderer,&player->destRect);
}

void renderNPC(SDL_Renderer* renderer,Game* game, NPC* npc) {
  // set the source rect coords
	npc->srcRect.x = npc->sprite->frame * npc->sprite->frameWidth;
	npc->srcRect.y = npc->sprite->animation * npc->sprite->frameHeight;

  // set the destination rect coords
	npc->destRect.x = (int)npc->x;
	npc->destRect.y = (int)npc->y;
	
  // blast the npc->sprite frame to the renderer target
	SDL_RenderCopyEx(
	  renderer,
    npc->texture,
    &npc->srcRect,
    &npc->destRect,
    0, // no rotation
    0, // no rotation center point (uses center of sprite if rotation above has a value)
    npc->sprite->hflip // not flipped
  );
}

void renderBoss(SDL_Renderer* renderer,Game* game, Boss* boss) {
  // set the source rect coords
	boss->srcRect.x = boss->sprite->frame * boss->sprite->frameWidth;
	boss->srcRect.y = boss->sprite->animation * boss->sprite->frameHeight;

  // set the destination rect coords
	boss->destRect.x = (int)boss->x;
	boss->destRect.y = (int)boss->y;
	
  // blast the boss->sprite frame to the renderer target
	SDL_RenderCopyEx(
	  renderer,
    boss->texture,
    &boss->srcRect,
    &boss->destRect,
    0, // no rotation
    0, // no rotation center point (uses center of sprite if rotation above has a value)
    boss->sprite->hflip // not flipped
  );
}

int getPlayerCollision(Game* game, Player* player) {
  // FIXME: need better approach to handling collisions.
  // seems to work fine for the top left corner of player, so look at
  // adding offset checks from the origin of the player.

  int tileX = (player->x) / TILE_WIDTH; //= 6
  int tileY = (player->y) / TILE_HEIGHT; //= 3

  // player tile i = tile x + tile y * map width
  int currTileIndex =  tileX + tileY * MAP_W;
  // printf("currTileIndex: %u \n", currTileIndex);
  printf("tileX: %u tileY: %u\n", tileX, tileY);
  printf("tileX * tileY: %u\n", currTileIndex);
  printf("gameMapArr[%u]: %u \n", currTileIndex, gameMapArr[currTileIndex]);
  fflush(stdout);


  return gameMapArr[currTileIndex];
}

void updatePlayScene(SDL_Renderer* renderer,Game* game, float dt) {
  // printf("UPDATE PLAY SCENE: %s\n", game->currentSceneName);
  
  // move npcs and bosses
  // game->maleOne->sprite->hflip = SDL_FLIP_HORIZONTAL;
  // game->maleOne->x += dt / 10;
  // printf("maleOne X: %u\n",game->maleOne->x);
  // updateSprite(game->maleOne->sprite, dt);

  if (game->gameInput->isKeyDown && game->gameInput->whichKey) {
    switch(game->gameInput->whichKey) {
      case SDL_SCANCODE_RIGHT: {
        game->player->sprite->animation = 9;
        game->player->x += dt / 10;
        if (getPlayerCollision(game, game->player)) {
          // game->player->x -= dt / 10;
          // printf("hit something \n");
          // fflush(stdout);
        }
        break;
      }
      case SDL_SCANCODE_LEFT: {
        game->player->sprite->animation = 11;        
        game->player->x -= dt / 20;
        if (getPlayerCollision(game, game->player)) {
          // game->player->x += dt / 20;
          
          // printf("hit something \n");
          // fflush(stdout);
        }
        break;
      }
      case SDL_SCANCODE_UP: {
        game->player->sprite->animation = 8;        
        game->player->y -= dt / 20;
        if (getPlayerCollision(game, game->player)) {
          // game->player->y += dt / 20;
          // printf("hit something \n");
          // fflush(stdout);
        }
        break;
      }
      case SDL_SCANCODE_DOWN: {
        game->player->sprite->animation = 10;        
        game->player->y += dt / 11;
        if (getPlayerCollision(game, game->player)) {
          // game->player->y -= dt / 11;
          // printf("hit something \n");
          // fflush(stdout);
        }
        break;
      }
      default: {
        break;
      }
    }
    updateSprite(game->player->sprite, dt);
  }
}

void update(SDL_Renderer* renderer, Game* game, float dt) { 
  // TODO: implement game update logic
  // printf("CURRENT SCENE: %s\n", game->currentSceneName);
  // fflush(stdout);
  if (game->updateFunc) {
    game->updateFunc(renderer,game,dt);
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
        game->gameInput->whichKey = 0;   
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
    destroyItem(game->waterCooler);
    destroyItem(game->copier);
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
  destRect.x = 100;
  destRect.y = 100;
  destRect.w = SPRITE_WIDTH;
  destRect.h = SPRITE_HEIGHT;

  player->srcRect = srcRect;
  player->destRect = destRect;
  player->x = 100;
  player->y = 100;
  player->health = 100;
  player->timeScore = 0;
  player->reportEff = 1;
  player->status = 1;
  player->texture = texture;
  player->sprite = newSprite();

  return player;
}

void destroyPlayer(Player* player) {
  if (player) {
    if (player->texture) {
      SDL_DestroyTexture(player->texture);
    }
    if (player->sprite) {
      destroySprite(player->sprite);
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
  destRect.x = 150;
  destRect.y = 200;
  destRect.w = SPRITE_WIDTH;
  destRect.h = SPRITE_HEIGHT;

  boss->srcRect = srcRect;
  boss->destRect = destRect;
  boss->x = 150;
  boss->y = 200;
  boss->health = 100;
  boss->status = 1;
  boss->texture = texture;
  boss->sprite = newSprite();

  return boss;
}

void destroyBoss(Boss* boss) {
  if (boss) {
    if (boss->texture) {
      SDL_DestroyTexture(boss->texture);
    }
    if (boss->sprite) {
      destroySprite(boss->sprite);
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
  npc->sprite = newSprite();

  return npc;
}

void destroyNPC(NPC* npc) {
  if (npc) {
    if (npc->texture) {
      SDL_DestroyTexture(npc->texture);
    }
    if (npc->sprite) {
      destroySprite(npc->sprite);
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

Sprite* newSprite() {
  Sprite* sprite = malloc(sizeof *sprite);
  sprite->animation = 9; // TODO: this is just for walking, add another for "interaction"
  sprite->frameWidth = SPRITE_WIDTH;
  sprite->frameHeight = SPRITE_HEIGHT;
  sprite->startFrame = 0;
  sprite->frame = 0;
  sprite->numFrames = 9; // TODO: this is just for walking, add another for "interaction"
  sprite->loop = 1; // 1 for is looping, 0 for not looping
  sprite->rate = 100;
  sprite->time = 0;
  sprite->hflip = SDL_FLIP_HORIZONTAL;  // left-facing: SDL_FLIP_NONE
  return sprite;
}

void destroySprite(Sprite* sprite) {
  if (sprite) {
    free(sprite);
    sprite = 0;
  }
}

TextRect* newTextRect(SDL_Renderer *renderer,int x, int y, int w, int h, int fontSize ,char* filename, char* text, int isShown, SDL_Color color) {
  TextRect* textRect = malloc(sizeof *textRect);

  textRect->font = loadText(filename, fontSize);

  SDL_Surface* surface = TTF_RenderText_Solid(textRect->font, text, color);

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
    if (textRect->texture) {
      SDL_DestroyTexture(textRect->texture);
    }
    free(textRect);
    textRect = 0;
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
    SHOW_TEXT,
    BLACK
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
    SHOW_TEXT,
    BLACK
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
    SHOW_TEXT,
    BLACK
  );

  game->currentSceneName = "title";
  game->updateFunc = &updateTitleScene;
  game->renderFunc = &renderTitleScene;

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
    SCREEN_WIDTH * 0.03,
    SCREEN_HEIGHT * 0.92,
    100,
    30, 
    50,
    "../assets/OpenSans-Bold.ttf",
    "HEALTH:",
    SHOW_TEXT,
    WHITE
  );
  
  if (game->currentScene) {
    destroyCurrentScene(game->currentScene, game->numSceneRects);
  }

  game->currentSceneName = "play";
  game->updateFunc = &updatePlayScene;
  game->renderFunc = &renderPlayScene;

  // set initial NPC, bosses, and player positions
  game->maleOne->x = 300;
  game->maleOne->y = 300;

  game->femaleOne->x = 200;
  game->femaleOne->y = 300;

  game->femaleTwo->x = 540;
  game->femaleTwo->y = 60;

  game->firstBoss->x = 100;
  game->firstBoss->y = 400;

  game->finalBoss->x = 300;
  game->finalBoss->y = 250;

  game->player->x = 100;
  game->player->y = 250;

  // set item positions
  game->copier->x = 500;
  game->copier->y = 380;

  game->waterCooler->x = 500;
  game->waterCooler->y = 50;

  game->computer->x = 50;
  game->computer->y = 30;

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
  SDL_Rect destRect;

  for (int i = 0; i < MAP_W * MAP_H; ++i) {
    destRect.x = (i % MAP_W) * TILE_WIDTH;
    destRect.y = (i / MAP_W) * TILE_HEIGHT;
    destRect.w = TILE_WIDTH;
    destRect.h = TILE_HEIGHT;
    
    if (gameMapArr[i] == 1) {
      renderMapTile(renderer, game, game->gameMap->wallTexture, &destRect);
    } else {
      renderMapTile(renderer, game, game->gameMap->groundTexture, &destRect);
    }
  };
}

void renderMapTile(SDL_Renderer *renderer, Game* game, SDL_Texture* texture, SDL_Rect* destRect) {
  SDL_RenderCopy(renderer, texture, NULL, destRect); 
}

void renderItem(SDL_Renderer *renderer, Game* game, Item* item) {
  SDL_Rect itemSrcRect;
  itemSrcRect.x = 0;
  itemSrcRect.y = 0;
  itemSrcRect.w = item->srcRect.w;
  itemSrcRect.h = item->srcRect.h;

  SDL_Rect itemDestRect;
  itemDestRect.x = item->x;
  itemDestRect.y = item->y;
  itemDestRect.w = item->destRect.w;
  itemDestRect.h = item->destRect.w;
  SDL_RenderCopyEx(renderer, item->texture, &itemSrcRect, &itemDestRect, 0, NULL, SDL_FLIP_NONE); 
}

void renderPlayScene(SDL_Renderer *renderer, Game* game) {
  // renders:
  // 1. Game Environment - TODO: in progress
  // 2. Player - TODO: in progress
  // 3. NPCs - TODO: in progress
  // 4. Bosses - TODO: in progress
  
  renderMap(renderer, game);
  renderItem(renderer,game, game->computer);
  renderItem(renderer,game, game->copier);
  renderItem(renderer,game, game->waterCooler);
  renderNPC(renderer, game, game->maleOne);
  renderNPC(renderer, game, game->femaleOne);
  renderNPC(renderer, game, game->femaleTwo);
  renderBoss(renderer, game, game->firstBoss);
  renderBoss(renderer, game, game->finalBoss);
  renderPlayer(renderer, game, game->player);
  
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
}

void renderTitleScene(SDL_Renderer *renderer, Game* game) {
  // renders text of scene - assuming scene has text
  SDL_SetRenderDrawColor(renderer, 178, 232, 255, 255);

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
}

void renderScene(SDL_Renderer *renderer, Game* game) {

  if (game->renderFunc) {
    game->renderFunc(renderer,game );
  }
}

TTF_Font* loadText(const char *fileName, int fontSize) {
  TTF_Font* font = TTF_OpenFont(fileName, fontSize); //this opens a font style and sets a size
  return font;
}

Game* newGame(SDL_Renderer* renderer) {

  Game* game = malloc(sizeof *game);

  game->gameInput = newGameInput();
  game->gameMap = newGameMap(renderer);
  game->player = newPlayer(renderer);
  game->inventory = newInventory();
  game->computer = newItem(renderer, "computer", "../assets/workstation-computer-55x50.png", COMPUTER_WIDTH, COMPUTER_HEIGHT);
  game->waterCooler = newItem(renderer, "computer", "../assets/workstation-watercooler-40x60.png", WATERCOOLER_WIDTH, WATERCOOLER_HEIGHT);
  game->copier = newItem(renderer, "computer", "../assets/workstation-copier-55x55.png", COPIER_WIDTH, COPIER_HEIGHT);
  game->firstBoss = newBoss(renderer, "../assets/first_boss.png");
  game->finalBoss = newBoss(renderer, "../assets/final_boss.png");
  game->maleOne = newNPC(renderer, "../assets/male_1.png");
  game->femaleOne = newNPC(renderer, "../assets/female_1.png");
  game->femaleTwo = newNPC(renderer, "../assets/female_2.png");
  game->currentScene = 0;
  game->renderFunc = 0;
  game->updateFunc = 0;

  return game;
}

void render(SDL_Renderer *renderer, Game* game) {
  // SDL_SetRenderDrawColor(renderer, 178, 232, 255, 255);
  SDL_RenderClear(renderer);

  renderScene(renderer,game);

  SDL_RenderPresent(renderer);
}

void cleanUp(SDL_Renderer *renderer,SDL_Window *window, Game* game) {
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

  if (SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT) != 0) {
    SDL_DestroyWindow(window);
    printf("SDL_RenderSetLogicalSize Error: %s \n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  // for allowing loading of PNG files. IMG_INIT_PNG == 2
  if(IMG_Init(IMG_INIT_PNG) != 2){
    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    return 1;
  }   
  
  SDL_SetRenderDrawColor(renderer, 178, 232, 255, 255);
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