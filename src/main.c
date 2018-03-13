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

char* globalTextBuffer[2048];

char updatePlayerHealthText = 0;
char updateBossHealthText = 0;
char updateTimeScoreText = 0;
char updateReportEffText = 0;
char updateReportAmmoText = 0;
char updateFightSceneActionText = 0;
char playerAttackChoice = -1;
char gameLevel = 0;
char startOver = 0;

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
  TextRect* menuChoice1;
  TextRect* menuChoice2;
  int whichMenu;
  int whichMenuChoice;
  int isActive;
} GameMenu;

typedef struct {
  int reportAmmo;
  int generatedReports;
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
  int hasAttacked;
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
  unsigned char isEnabled;
} Boss;

typedef struct {
  char* itemName;
  SDL_Texture* texture;  
  SDL_Rect srcRect;
  SDL_Rect destRect;
  int x;
  int y;
  unsigned char isEnabled;
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
  unsigned char isEnabled;
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
  GameMenu* gameMenu;
  TextRect* currentScene;
  Player* player;
  Boss* firstBoss;
  Boss* finalBoss;
  NPC* maleOne;
  NPC* femaleOne;
  NPC* femaleTwo;
  Inventory* inventory;
  Item* computer;
  Item* waterCooler;
  Item* copier;
  int fightDelay;
  char* currentSceneName;
  void (*updateFunc)(SDL_Renderer* renderer, struct t_Game* game, float dt);
  void (*renderFunc)(SDL_Renderer* renderer, struct t_Game* game);
} Game;

// Forward Declarations
void bossAttack(SDL_Renderer* renderer,Game* game);
void cleanUp(SDL_Renderer *renderer,SDL_Window *window, Game* game);

void destroyGame(Game* game);
void destroyGameInput(GameInput* gameInput);
void destroyGameMap(GameMap* gameMap);
void destroyInventory(Inventory* inventory);
void destroyGameMenu(GameMenu* gameMenu);
void destroyItem(Item* item);
void destroyPlayer(Player* player);
void destroyBoss(Boss* boss);
void destroyNPC(NPC* npc);
void destroyCurrentScene(TextRect* textRect, int numSceneRects);
void destroySprite(Sprite* sprite);
void destroyTextRect(TextRect* textRect);

int initGame(void);
int getPlayerCollision(Game* game, Player* player);

Boss* newBoss(SDL_Renderer* renderer, char* filename);
TextRect* newCurrentScene(int numSceneRects);
Item* newItem(SDL_Renderer* renderer, char* itemName, char* filename, int width, int height);
Game* newGame(SDL_Renderer* renderer);
GameInput* newGameInput();
GameMap* newGameMap(SDL_Renderer* renderer);
GameMenu* newGameMenu(SDL_Renderer* renderer, Game* game, int isActive, int whichMenu);
Inventory* newInventory();
NPC* newNPC(SDL_Renderer* renderer, char* filename);
Player* newPlayer(SDL_Renderer* renderer);
Sprite* newSprite();

TextRect* newTextRect(SDL_Renderer* renderer, int x, int y, int w, int h, int fontSize ,char* filename, char* text, int isShown, SDL_Color color);

void handleInput(SDL_Renderer* renderer,Game* game, int *loop);
void handleMouseDown(SDL_Renderer* renderer,Game* game);
void handleMouseMove(SDL_Renderer* renderer,Game* game);
void handleWhichKey(Game* game, SDL_Keysym *keysym);
int hitBoss(Boss* boss, Player *player);
int hitItem(Item* item, Player *player, int itemWidth, int itemHeight);
int hitNPC(NPC* npc, Player *player);

void updateTitleScene(SDL_Renderer* renderer, Game* game, float dt);
void updateNextLevelScene(SDL_Renderer* renderer, Game* game, float dt);
void updateTextRect(SDL_Renderer* renderer, TextRect* textRect, int nextVal, SDL_Color color);
void updateFightSceneStatusText (SDL_Renderer *renderer, TextRect* textRect, char* newText, SDL_Color color);
void updatePlayScene(SDL_Renderer* renderer, Game* game, float dt);
void updateFightScene(SDL_Renderer* renderer, Game* game, float dt);
void updateSprite(Sprite* sprite, float dt);
void updateGameOverScene(SDL_Renderer* renderer, Game* game, float dt);
void updateGameWinScene(SDL_Renderer* renderer, Game* game, float dt);

void loadFightScene(SDL_Renderer* renderer,Game* game);
void loadGameOverScene(SDL_Renderer* renderer, Game* game);
void loadGameWinScene(SDL_Renderer* renderer,Game* game);
void loadNextLevelScene(SDL_Renderer* renderer,Game* game);
void loadPlayScene(SDL_Renderer* renderer, Game* game);
TTF_Font* loadText(const char *fileName, int fontSize);
void loadTitleScene(SDL_Renderer* renderer, Game* game);

void playerAttack(SDL_Renderer* renderer, Game* game, int playerAttackChoice, int gameLevel);

void renderText(SDL_Renderer *renderer, SDL_Texture *texture,TTF_Font *font,SDL_Rect rect, char *text);
void render(SDL_Renderer *renderer, Game* game);
void renderMap(SDL_Renderer * renderer, Game* game);
void renderMapTile(SDL_Renderer *renderer, Game* game, SDL_Texture* texture, SDL_Rect* destRect);
void renderPlayer(SDL_Renderer *renderer, Game* game, Player* player);
void renderNPC(SDL_Renderer *renderer, Game* game, NPC* npc);
void renderBoss(SDL_Renderer *renderer, Game* game, Boss* boss);
void renderScene(SDL_Renderer *renderer,Game* game);
void renderPlayScene(SDL_Renderer *renderer, Game* game);
void renderFightScene(SDL_Renderer *renderer, Game* game);
void renderTitleScene(SDL_Renderer *renderer, Game* game);
void renderNextLevelScene(SDL_Renderer *renderer, Game* game);
void renderGameWinScene(SDL_Renderer *renderer, Game* game);
void renderGameOverScene(SDL_Renderer *renderer, Game* game);
void renderGameMenu(SDL_Renderer *renderer, Game* game);

void update(SDL_Renderer* renderer,Game* game, float dt);
void updateText(SDL_Renderer *renderer, SDL_Texture *texture,TTF_Font *font,SDL_Rect rect, char *text);

// Function Definitions
void renderGameMenu(SDL_Renderer *renderer, Game* game) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  SDL_Rect rect;
  rect.x = game->gameMenu->menuChoice1->rect.x;
  rect.y = game->gameMenu->menuChoice1->rect.y;
  rect.w = game->gameMenu->menuChoice2->rect.w;
  rect.h = game->gameMenu->menuChoice1->rect.h + game->gameMenu->menuChoice2->rect.h;
  SDL_RenderDrawRect(renderer, &rect);
  SDL_SetRenderDrawColor(renderer, 178, 232, 255, 255);
  if ( game->gameMenu->whichMenuChoice == 0) {
    SDL_Rect rect1;
    rect1.x = game->gameMenu->menuChoice1->rect.x;
    rect1.y = game->gameMenu->menuChoice1->rect.y;
    rect1.w = game->gameMenu->menuChoice2->rect.w;
    rect1.h = game->gameMenu->menuChoice1->rect.h;
    SDL_RenderFillRect(renderer,&rect1);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);

  } else if (game->gameMenu->whichMenuChoice == 1) {
    SDL_RenderFillRect(renderer,&game->gameMenu->menuChoice2->rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
  }
  renderText(
    renderer, 
    game->gameMenu->menuChoice1->texture,
    game->gameMenu->menuChoice1->font,  
    game->gameMenu->menuChoice1->rect, 
    game->gameMenu->menuChoice1->text
  );
  renderText(
    renderer, 
    game->gameMenu->menuChoice2->texture,
    game->gameMenu->menuChoice2->font,  
    game->gameMenu->menuChoice2->rect, 
    game->gameMenu->menuChoice2->text
  );
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  
}


GameMenu* newGameMenu(SDL_Renderer* renderer,Game* game,int isActive, int whichMenu) {
  // TODO: could refactor to allow for multiple menus simply, per scene
  // isActive:
  // 0 --> hidden and not responsive (probably obvious...)
  // 1 --> active and responsive to mouse location
  // whichMenu:
  // 0 --> play scene computer menu
  // 1 --> fight scene player menu
  // whichMenuChoice:
  // 0 --> top choice
  // 1 --> bottom choice

  GameMenu* gameMenu = malloc(sizeof *gameMenu);
  if (whichMenu == 0) {
    gameMenu->menuChoice1 = newTextRect(
      renderer,
      game->computer->x + 100,
      game->computer->y + 5,
      160,
      50, 
      60,
      "../assets/OpenSans-Bold.ttf",
      "Generate Reports",
      SHOW_TEXT,
      BLACK
    );
    gameMenu->menuChoice2 = newTextRect(
      renderer,
      game->computer->x + 100,
      game->computer->y + 50,
      190,
      50, 
      60,
      "../assets/OpenSans-Bold.ttf",
      "Develop WCB Scripts",
      SHOW_TEXT,
      BLACK
    );
  } else if (whichMenu == 1) {
    gameMenu->menuChoice1 = newTextRect(
      renderer,
      game->player->x + 340,
      game->player->y + 150,
      160,
      50,
      80,
      "../assets/OpenSans-Bold.ttf",
      "Reports Attack",
      SHOW_TEXT,
      BLACK
    );
    gameMenu->menuChoice2 = newTextRect(
      renderer,
      game->player->x + 340,
      game->player->y + 195,
      190,
      50,
      80,
      "../assets/OpenSans-Bold.ttf",
      "Tactical BS",
      SHOW_TEXT,
      BLACK
    );
  }

  gameMenu->whichMenu = whichMenu;
  gameMenu->whichMenuChoice = -1;
  gameMenu->isActive = isActive;
  return gameMenu;
}

void destroyGameMenu(GameMenu* gameMenu) {
  if (gameMenu) {
    if (gameMenu->menuChoice1) {
      destroyTextRect(gameMenu->menuChoice1);
    }
    if (gameMenu->menuChoice2) {
      destroyTextRect(gameMenu->menuChoice2);
    }
    free(gameMenu);
  }
}

void updateFightScene(SDL_Renderer* renderer, Game* game, float dt) {
  // TODO: connect battle menu choices to affect player and boss health
  handleMouseMove(renderer, game);

  if (game->player->hasAttacked) {
    // slight delay before boss attack
    game->fightDelay -= dt;
    if (game->fightDelay <= 0) {
      game->fightDelay = 700;
      // boss attack
      bossAttack(renderer,game);
    }
  }

  if (updatePlayerHealthText == 0) {
    updateTextRect(renderer, &game->currentScene[0],game->player->health, BLACK);
    updatePlayerHealthText = 1;
  }

  if (updateFightSceneActionText == 0) {
    updateFightSceneStatusText(renderer, &game->currentScene[2],game->currentScene[2].text, BLACK);
    updateFightSceneActionText = 1;
  }

  // make this adapt to either final boss or first boss - same goes for loadFightScene
  if (gameLevel == 0) {
    // handle first boss health text here
    if (updateBossHealthText == 0) {
      updateTextRect(renderer, &game->currentScene[1],game->firstBoss->health, BLACK);
      updateBossHealthText = 1;
    }
  // TODO: adapt for more bosses... ?
  } else if (gameLevel == 1) {
    if (updateBossHealthText == 0) {
      updateTextRect(renderer, &game->currentScene[1],game->finalBoss->health, BLACK);
      updateBossHealthText = 1;
    }
  }
}

void renderFightScene(SDL_Renderer *renderer, Game* game) {

  if (gameLevel == 0) {
    renderBoss(renderer, game, game->firstBoss);
  } else if (gameLevel == 1) {
    renderBoss(renderer, game, game->finalBoss);
  }
  renderPlayer(renderer, game, game->player);

  if (game->gameMenu->isActive) {
    renderGameMenu(renderer, game);
  }

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

void updateTitleScene(SDL_Renderer* renderer,Game* game, float dt) {
  // printf("UPDATE TITLE SCENE: %s\n", game->currentSceneName); 

  if (startOver == 1) {
    startOver = 0;
    game->gameInput->whichKey = -1;
    return;
  } 

  if (game->gameInput->whichKey == SDL_SCANCODE_RETURN) {

    SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
    game->finalBoss->srcRect.w = SPRITE_WIDTH;
    game->finalBoss->srcRect.h = SPRITE_HEIGHT;
    game->finalBoss->destRect.w = SPRITE_WIDTH;
    game->finalBoss->destRect.h = SPRITE_HEIGHT;
    
    game->firstBoss->srcRect.w = SPRITE_WIDTH;
    game->firstBoss->srcRect.h = SPRITE_HEIGHT;
    game->firstBoss->destRect.w = SPRITE_WIDTH;
    game->firstBoss->destRect.h = SPRITE_HEIGHT;
    loadPlayScene(renderer,game);
  }
}

void updateNextLevelScene(SDL_Renderer* renderer,Game* game, float dt) {
  // printf("UPDATE TITLE SCENE: %s\n", game->currentSceneName);  

  if (game->gameInput->whichKey == SDL_SCANCODE_RETURN) {
    game->player->timeScore = 0;
    game->inventory->generatedReports = 0;
    game->inventory->reportAmmo = 0;
    game->player->reportEff += 1;
    game->player->srcRect.w = SPRITE_WIDTH;
    game->player->srcRect.h = SPRITE_HEIGHT;
    game->player->destRect.w = SPRITE_WIDTH;
    game->player->destRect.h = SPRITE_HEIGHT;

    updatePlayerHealthText = 0;
    updateReportAmmoText = 0;
    updateTimeScoreText = 0;
    updateReportEffText = 0;
    updateBossHealthText = 0;
    
    SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
    loadPlayScene(renderer,game);
  }
}

void updateGameWinScene(SDL_Renderer* renderer,Game* game, float dt) {
  // printf("UPDATE TITLE SCENE: %s\n", game->currentSceneName);  

  if (game->gameInput->whichKey == SDL_SCANCODE_RETURN) {
    game->player->timeScore = 0;
    game->inventory->generatedReports = 0;
    game->inventory->reportAmmo = 0;
    game->player->reportEff += 1;
    game->player->srcRect.w = SPRITE_WIDTH;
    game->player->srcRect.h = SPRITE_HEIGHT;
    game->player->destRect.w = SPRITE_WIDTH;
    game->player->destRect.h = SPRITE_HEIGHT;

    game->finalBoss->srcRect.w = SPRITE_WIDTH;
    game->finalBoss->srcRect.h = SPRITE_HEIGHT;
    game->finalBoss->destRect.w = SPRITE_WIDTH;
    game->finalBoss->destRect.h = SPRITE_HEIGHT;

    game->firstBoss->health = 100;
    game->finalBoss->health = 100;
    game->player->health = 100;

    updatePlayerHealthText = 0;
    updateReportAmmoText = 0;
    updateTimeScoreText = 0;
    updateReportEffText = 0;
    updateBossHealthText = 0;
    game->firstBoss->sprite->animation = 11;
    game->firstBoss->sprite->frame = 0;
    game->finalBoss->sprite->animation = 11;
    game->finalBoss->sprite->frame = 0;

    SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
    gameLevel = 0;

    startOver = 1;
    loadTitleScene(renderer,game);
    
  }
}

void updateGameOverScene(SDL_Renderer* renderer,Game* game, float dt) {
  // printf("UPDATE TITLE SCENE: %s\n", game->currentSceneName);  

  if (game->gameInput->whichKey == SDL_SCANCODE_RETURN) {
    game->player->timeScore = 0;
    game->inventory->generatedReports = 0;
    game->inventory->reportAmmo = 0;
    game->player->reportEff += 1;
    game->player->srcRect.w = SPRITE_WIDTH;
    game->player->srcRect.h = SPRITE_HEIGHT;
    game->player->destRect.w = SPRITE_WIDTH;
    game->player->destRect.h = SPRITE_HEIGHT;

    game->finalBoss->srcRect.w = SPRITE_WIDTH;
    game->finalBoss->srcRect.h = SPRITE_HEIGHT;
    game->finalBoss->destRect.w = SPRITE_WIDTH;
    game->finalBoss->destRect.h = SPRITE_HEIGHT;

    game->firstBoss->health = 100;
    game->finalBoss->health = 100;
    game->player->health = 100;

    updatePlayerHealthText = 0;
    updateReportAmmoText = 0;
    updateTimeScoreText = 0;
    updateReportEffText = 0;
    updateBossHealthText = 0;
    game->firstBoss->sprite->animation = 11;
    game->firstBoss->sprite->frame = 0;
    game->finalBoss->sprite->animation = 11;
    game->finalBoss->sprite->frame = 0;

    SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
    gameLevel = 0;

    startOver = 1;
    loadTitleScene(renderer,game);
    
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

	// TODO: remove this, this is for helping with collision detection
  // SDL_RenderDrawRect(renderer,&player->destRect);
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
}

void renderNPC(SDL_Renderer* renderer,Game* game, NPC* npc) {
  // set the source rect coords
	npc->srcRect.x = npc->sprite->frame * npc->sprite->frameWidth;
	npc->srcRect.y = npc->sprite->animation * npc->sprite->frameHeight;

  // set the destination rect coords
	npc->destRect.x = (int)npc->x;
	npc->destRect.y = (int)npc->y;
	
  // blast the npc->sprite frame to the renderer target
	// TODO: remove this, this is for helping with collision detection
  if (npc->isEnabled) {
    SDL_RenderFillRect(renderer,&npc->destRect);
  }

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

  // TODO: remove this, this is for helping with collision detection
  if (boss->isEnabled) {
    SDL_RenderFillRect(renderer,&boss->destRect);
  }
	
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

int hitBoss(Boss* boss, Player *player) {
  if (player->x + SPRITE_WIDTH >= boss->x && player->x <= boss->x + SPRITE_WIDTH) {
    if (player->y + SPRITE_HEIGHT >= boss->y && player->y <= boss->y + SPRITE_HEIGHT) {
      // printf("hit firstBoss\n");
      // fflush(stdout);
      return 1;
    }
  }
  return 0;
}

int hitItem(Item* item, Player *player, int itemWidth, int itemHeight) {
  if (player->x + SPRITE_WIDTH >= item->x && player->x <= item->x + itemWidth) {
    if (player->y + itemHeight >= item->y && player->y <= item->y + itemHeight) {
      // printf("hit firstBoss\n");
      // fflush(stdout);
      return 1;  
    }
  }
  return 0;
}

int hitNPC(NPC* npc, Player *player) {
  if (player->x + SPRITE_WIDTH >= npc->x && player->x <= npc->x + SPRITE_WIDTH) {
    if (player->y + SPRITE_HEIGHT >= npc->y && player->y <= npc->y + SPRITE_HEIGHT) {
      // printf("hit firstBoss\n");
      // fflush(stdout);
      return 1;  
    }
  }
  return 0;
}

int getPlayerCollision(Game* game, Player* player) {

  int tileX1 = (player->x + SPRITE_WIDTH * 0.25) / TILE_WIDTH;
  int tileX2 = (player->x + SPRITE_WIDTH * 0.75) / TILE_WIDTH;
  int tileY1 = (player->y) / TILE_HEIGHT;
  int tileY2 = (player->y + SPRITE_HEIGHT) / TILE_HEIGHT;
  // player tile i = tile x + tile y * map width
  int currTileIndex1 =  tileX1 + tileY1 * MAP_W;
  int currTileIndex2 =  tileX1 + tileY2 * MAP_W;
  int currTileIndex3 =  tileX2 + tileY1 * MAP_W;
  int currTileIndex4 =  tileX2 + tileY2 * MAP_W;

  // TODO: these object collision only work for detecting the play
  if (hitNPC(game->maleOne, player)) {
    game->maleOne->isEnabled = 1;
  }

  if (hitNPC(game->femaleOne, player)) {
    game->femaleOne->isEnabled = 1;
  }

  if (hitNPC(game->femaleTwo, player)) {
    game->femaleTwo->isEnabled = 1;
  }

  if (hitBoss(game->firstBoss, player)) {
    game->firstBoss->isEnabled = 1;  
  }

  if (hitBoss(game->finalBoss, player)) {
    game->finalBoss->isEnabled = 1;  
  }

  if (hitItem(game->copier, player, COPIER_WIDTH, COPIER_HEIGHT)) {
    game->copier->isEnabled = 1;
  }

  if (hitItem(game->waterCooler, player, WATERCOOLER_WIDTH, WATERCOOLER_HEIGHT)) {
    game->waterCooler->isEnabled = 1;
  }

  if (hitItem(game->computer, player, COMPUTER_WIDTH, COMPUTER_HEIGHT)) {
    game->computer->isEnabled = 1;
    if (game->gameMenu) {
      game->gameMenu->isActive = 1;
    }
  }

  // TODO: if time, improve this collision detection. good enough for now
  if (gameMapArr[currTileIndex1] || gameMapArr[currTileIndex2] || gameMapArr[currTileIndex3] || gameMapArr[currTileIndex4]) {
    return 1;
  }

  return 0 ;
}

void updatePlayScene(SDL_Renderer* renderer,Game* game, float dt) {
  // printf("UPDATE PLAY SCENE: %s\n", game->currentSceneName);
  
  // move npcs and bosses
  // game->maleOne->sprite->hflip = SDL_FLIP_HORIZONTAL;
  // game->maleOne->x += dt / 10;
  // printf("maleOne X: %u\n",game->maleOne->x);
  // updateSprite(game->maleOne->sprite, dt);
  handleMouseMove(renderer, game);

  if (updatePlayerHealthText == 0) {
    updateTextRect(renderer, &game->currentScene[0],game->player->health, WHITE);
    updatePlayerHealthText = 1;
  }

  if (updateTimeScoreText == 0) {
    updateTextRect(renderer, &game->currentScene[1],game->player->timeScore, WHITE);
    updateTimeScoreText = 1;
  }

  if (updateReportEffText == 0) {
    updateTextRect(renderer, &game->currentScene[2],game->player->reportEff, WHITE);
    updateReportEffText = 1;
  }
  if (updateReportAmmoText == 0) {
    updateTextRect(renderer, &game->currentScene[3],game->inventory->reportAmmo, WHITE);
    updateReportAmmoText = 1;
  }

  if (game->gameInput->isKeyDown && game->gameInput->whichKey) {
    game->femaleOne->isEnabled = 0;
    game->maleOne->isEnabled = 0;
    game->femaleTwo->isEnabled = 0;
    game->firstBoss->isEnabled = 0;
    game->copier->isEnabled = 0;
    game->waterCooler->isEnabled = 0;
    game->computer->isEnabled = 0;
    game->finalBoss->isEnabled = 0;   
    game->gameMenu->isActive = 0;
    switch(game->gameInput->whichKey) {
      case SDL_SCANCODE_RIGHT: {
        game->player->sprite->animation = 9;
        game->player->x += 2; 

        if (getPlayerCollision(game, game->player)) {
          game->player->x -= 2;
          // printf("hit something \n");
          // fflush(stdout);
        }
        break;
      }
      case SDL_SCANCODE_LEFT: {
        game->player->sprite->animation = 11;        
        game->player->x -= 2;
        if (getPlayerCollision(game, game->player)) {
          game->player->x += 2;
          
          // printf("hit something \n");
          // fflush(stdout);
        }
        break;
      }
      case SDL_SCANCODE_UP: {
        game->player->sprite->animation = 8;        
        game->player->y -= 2;
        if (getPlayerCollision(game, game->player)) {
          game->player->y += 2;
          // printf("hit something \n");
          // fflush(stdout);
        }
        break;
      }
      case SDL_SCANCODE_DOWN: {
        game->player->sprite->animation = 10;        
        game->player->y += 2;
        if (getPlayerCollision(game, game->player)) {
          game->player->y -= 2;
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

// NOTE: currently only deals with game menus
void handleMouseMove(SDL_Renderer* renderer, Game* game) {
  if (game->gameMenu->isActive) {
    if ( game->gameInput->mouseX >= game->gameMenu->menuChoice1->rect.x 
        && game->gameInput->mouseX <= game->gameMenu->menuChoice1->rect.x + game->gameMenu->menuChoice1->rect.w ) {
        if ( game->gameInput->mouseY >= game->gameMenu->menuChoice1->rect.y 
          && game->gameInput->mouseY <= game->gameMenu->menuChoice1->rect.y + game->gameMenu->menuChoice1->rect.h ) {
            // printf("on top choice\n");
            // fflush(stdout);      
            game->gameMenu->whichMenuChoice = 0;      
            return;
        }
    }
    if ( game->gameInput->mouseX >= game->gameMenu->menuChoice2->rect.x 
        && game->gameInput->mouseX <= game->gameMenu->menuChoice2->rect.x + game->gameMenu->menuChoice2->rect.w ) {
        if ( game->gameInput->mouseY >= game->gameMenu->menuChoice2->rect.y 
          && game->gameInput->mouseY <= game->gameMenu->menuChoice2->rect.y + game->gameMenu->menuChoice2->rect.h ) {
            // printf("on bottom choice\n");
            // fflush(stdout);
            game->gameMenu->whichMenuChoice = 1;
            return;
        }
    } else {
      // printf("outside choice\n");
      // fflush(stdout);
      game->gameMenu->whichMenuChoice = - 1;
      return;
    }
  } 
}

void handleMouseDown(SDL_Renderer* renderer,Game* game) {
  if (!game->gameInput->isMouseDown) {
    return;
  }

  if (game->computer->isEnabled) {
    // printf("add +1 to generated reports\n");
    // fflush(stdout);
    // just show the game menu now
    if (game->gameMenu->isActive) {
      if (game->gameMenu->whichMenuChoice == 0) {
        if (game->inventory->generatedReports < 5) {
          game->inventory->generatedReports += 1;
          // updateReportAmmoText = 0;
        }
        if (game->player->timeScore < 5) {
          game->player->timeScore += 1;
          updateTimeScoreText = 0;
        }
      } else if (game->gameMenu->whichMenuChoice == 1) {
          game->player->reportEff += 1;
        if (game->player->reportEff <= 2) {
          game->player->reportEff = 2;
          updateReportEffText = 0;
        }
        if (game->player->timeScore <= 2) {
          game->player->timeScore += 1;
          updateTimeScoreText = 0;
        }
      }
    }
    return;
  }

  if (game->femaleOne->isEnabled) {
    printf("add +1 to time score: femaleOne Conversation\n");
    game->player->timeScore += 1;
    if (game->player->timeScore >= 8) {
      game->player->timeScore = 8;
      updateTimeScoreText = 0;
      printf(" ***femaleOne speaking: go to boss now!***\n");
    } else {
      printf("**femaleOne making convo**\n");
      updateTimeScoreText = 0;
    }
    fflush(stdout);
    return;
  }
  if (game->femaleTwo->isEnabled) {
    printf("add +1 to time score: femaleTwo Conversation\n");
    game->player->timeScore += 1;
    if (game->player->timeScore >= 8) {
      game->player->timeScore = 8;
      updateTimeScoreText = 0;
      printf(" ***femaleTwo speaking: go to boss now!***\n");
    } else {
      printf("**femaleTwo making convo**\n");
      updateTimeScoreText = 0;
    }
    fflush(stdout);
    return;
  }
  if (game->maleOne->isEnabled) {
    printf("add +1 to time score: maleOne Conversation\n");
    game->player->timeScore += 1;
    if (game->player->timeScore >= 8) {
      game->player->timeScore = 8;
      updateTimeScoreText = 0;
      printf(" ***maleOne speaking: go to boss now!***\n");
    } else {
      printf("**maleOne making convo**\n");
      updateTimeScoreText = 0;
    }
    fflush(stdout);
    return;
  }
  if (game->copier->isEnabled) {
    printf("add +1 to report ammo\n");
    fflush(stdout);
    if (game->inventory->generatedReports <= 0) {
      printf("need to generate reports first!\n");
      fflush(stdout);
      game->inventory->generatedReports = 0;
      return;
    }
    if (game->inventory->reportAmmo < 5) {
      game->inventory->generatedReports -= 1;
      printf("add +1 to report ammo and -1 to gener. reports\n");
      fflush(stdout);
      game->inventory->reportAmmo += 1;
      updateReportAmmoText = 0;
    }
    return;
  }
  if (game->waterCooler->isEnabled) {
    game->player->health += 20;
    if (game->player->health >= 100) {
      game->player->health = 100;
    }
    updatePlayerHealthText = 0;
    return;
  }
  if (game->firstBoss->isEnabled) {
    printf("time score: %d", game->player->timeScore);
    printf("report Ammo: %d", game->inventory->reportAmmo);
    if (game->player->timeScore == 8 && game->inventory->reportAmmo == 5) {
      printf("enter first boss fight scene - win and beat level\n");
      fflush(stdout);
      game->firstBoss->isEnabled = 0;
      loadFightScene(renderer, game);
    } else if (game->player->timeScore != 8) {
      printf("increase times score first before -firstBoss- !\n");
      fflush(stdout);
    } else if (game->inventory->reportAmmo != 5) {
      printf("increase report ammo first before -firstBoss- !\n");
      fflush(stdout);   
    }
    return;
  }
  if (game->finalBoss->isEnabled) {
    printf("time score: %d\n", game->player->timeScore);
    printf("report Ammo: %d\n", game->inventory->reportAmmo);
    printf("first boss health: %d\n", game->firstBoss->health);
    printf("game level: %d\n", gameLevel);
    fflush(stdout);

    if (game->player->timeScore == 8 && game->firstBoss->health <= 0 && gameLevel == 1) {
      printf("enter final boss fight scene - win and beat game\n");
      fflush(stdout);
      game->player->hasAttacked = 0;
      game->finalBoss->isEnabled = 0;
      gameLevel = 1;
      loadFightScene(renderer, game);
      // TODO:  load final boss fight scene here
    } else if (game->firstBoss->health > 0) {
      printf("beat first boss before -finalboss- !\n");
      fflush(stdout);
    } else if (game->player->timeScore != 8 || game->inventory->reportAmmo != 5){
      printf("increase times score first or get more report ammo before -finalboss- !\n");
      fflush(stdout);
    }
    printf("nothing?");
    fflush(stdout);
    return;
  }

  // TODO: here - where fight scene actions can occur
  if (game->gameMenu && game->gameMenu->whichMenu == 1) {
    if (game->gameMenu->whichMenuChoice == 0) {
      // TODO: minus boss health by 20 X reportEff multiplier
      if (!game->player->hasAttacked) {
        playerAttack(renderer, game, 0, gameLevel);
      }
      return;
    }
    if (game->gameMenu->whichMenuChoice == 1) {
      // TODO: stun boss, causing him/her to not attack the next turn, and for their attacks
      // to be %15 less harmful
      if (!game->player->hasAttacked) {
        playerAttack(renderer, game, 1, gameLevel);
      }
      return;
    }
  }

}

void bossAttack(SDL_Renderer* renderer,Game* game) {
  // bossAttackHitChance: 0 = miss, 1 = partial hit, 2 = hit
  // bossAttackChoice: 0 = "Questions Integrity", 1 = "Bad Joke", 2 = "Found Report Problem"
  int bossAttackHitChance = rand() % (0 + 2 - 0) + 0;
  int bossAttackChoice = rand() % (2 + 1 - 0) + 0;
  switch(bossAttackChoice) {
    case 0: {
      // player missed
      printf("boss uses 'Questions Integrity'\n");
      fflush(stdout);
      switch(bossAttackHitChance) {
        case 0: {
          // player missed
          printf("boss missed!\n");
          fflush(stdout);
          game->currentScene[2].text = "boss uses 'Questions Integrity' - missed Attack!";
          updateFightSceneActionText = 0;
          break;
        }
        case 1: {
          // player attack partial hit
          printf("boss partial hit attack!\n");
          fflush(stdout);
          game->currentScene[2].text = "boss uses 'Questions Integrity' - partial hit!";
          game->player->health -= 10;
          if (game->player->health <= 0) {
            printf("game over");
            fflush(stdout);
            loadGameOverScene(renderer, game);
            return;
          }
          updatePlayerHealthText = 0;
          updateFightSceneActionText = 0;
          break;
        }
        case 2: {
          // player hit
          printf("boss direct hit!\n");
          fflush(stdout);
          game->currentScene[2].text = "boss uses 'Questions Integrity' - direct hit!";
          game->player->health -= 20;
          if (game->player->health <= 0) {
            printf("game over");
            fflush(stdout);
            loadGameOverScene(renderer, game);
            return;
          }
          updatePlayerHealthText = 0;
          updateFightSceneActionText = 0;
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case 1: {
      // boss attack - bad joke
      printf("boss uses 'Bad Joke'\n");
      fflush(stdout);
      switch(bossAttackHitChance) {
        case 0: {
          // boss missed
          printf("boss missed!\n");
          fflush(stdout);
          game->currentScene[2].text = "boss uses: 'Bad Joke' - missed!";
          updateFightSceneActionText = 0;
          break;
        }
        case 1: {
          // player attack partial hit
          printf("boss partial hit attack!\n");
          fflush(stdout);
          game->currentScene[2].text = "boss uses: 'Bad Joke' - partial hit!";
          game->player->health -= 10;
          if (game->player->health <= 0) {
            printf("-game over-\n");
            fflush(stdout);
            loadGameOverScene(renderer, game);
            return;
          }
          updatePlayerHealthText = 0;
          updateFightSceneActionText = 0;
          break;
        }
        case 2: {
          // player hit
          printf("boss direct hit!\n");
          fflush(stdout);
          game->currentScene[2].text = "boss uses: 'Bad Joke' - direct hit!";
          game->player->health -= 20;
          if (game->player->health <= 0) {
            printf("-game over-\n");
            fflush(stdout);
            loadGameOverScene(renderer, game);
            return;
          }
          updatePlayerHealthText = 0;
          updateFightSceneActionText = 0;
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case 2: {
      // player hit
      printf("boss uses 'Found Report Problem'\n");
      fflush(stdout);
      switch(bossAttackHitChance) {
        case 0: {
          // player missed
          game->currentScene[2].text = "boss uses: 'Found Report Problem' - missed!";
          updatePlayerHealthText = 0;
          updateFightSceneActionText = 0;
          break;
        }
        case 1: {
          // player attack partial hit
          game->currentScene[2].text = "boss uses: 'Found Report Problem' - partial hit!";
          game->player->health -= 10;
          if (game->player->health <= 0) {
            // enter game over scene
            printf("game over \n");
            fflush(stdout);
            loadGameOverScene(renderer, game);
            return;
          }
          updatePlayerHealthText = 0;
          updateFightSceneActionText = 0;
          break;
        }
        case 2: {
          // player hit
          game->currentScene[2].text = "boss uses: 'Found Report Problem' - direct hit!";
          game->player->health -= 20;
          if (game->player->health <= 0) {
            // enter game over scene
            printf("game over \n");
            fflush(stdout);
            loadGameOverScene(renderer, game);
            return;
          }
          updatePlayerHealthText = 0;
          updateFightSceneActionText = 0;
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    default: {
      break;
    }
  }
  game->player->hasAttacked = 0;
}

void playerAttack(SDL_Renderer* renderer, Game* game, int playerAttackChoice, int gameLevel) {
  // playerAttackChoice: 0 = report attack, 1 = tactical BS
  // playerAttackHitChance: 0 = miss, 1 = partial hit (50% damage), 2 = hit

  int playerAttackHitChance = rand() % (2 + 1 - 0) + 0;

  switch(playerAttackChoice) {
    case 0: {
      // player missed
      printf("report attack - attack!\n");
      fflush(stdout);
      switch(playerAttackHitChance) {
        case 0: {
          // player missed
          game->currentScene[2].text = "You use: 'Report Attack' - missed!";
          updateFightSceneActionText = 0;
          break;
        }
        case 1: {
          // player attack partial hit
          game->currentScene[2].text = "You use: 'Report Attack' - partial hit!";
          updateFightSceneActionText = 0;
          if (gameLevel == 0) {
            game->firstBoss->health -= 15;
            if (game->firstBoss->health <= 0) {
              // enter next level scene
              printf("next level \n");
              fflush(stdout);
              gameLevel = 1;
              updateBossHealthText = 0;
              updateFightSceneActionText = 0;
              loadNextLevelScene(renderer, game);
            }
          } else if (gameLevel == 1) {
            game->finalBoss->health -= 15;
            if (game->finalBoss->health <= 0) {
              // enter game win screen
              printf("game won \n");
              fflush(stdout);
              loadGameWinScene(renderer, game);
            }
          }
          updateBossHealthText = 0;
          break;
        }
        case 2: {
          // player hit
          game->currentScene[2].text = "You use: 'Report Attack' - direct hit!";
          updateFightSceneActionText = 0;
          if (gameLevel == 0) {
            game->firstBoss->health -= 30;
            if (game->firstBoss->health <= 0) {
              // enter next level scene
              printf("next level \n");
              fflush(stdout);
              gameLevel = 1;
              updateBossHealthText = 0;
              updateFightSceneActionText = 0;
              loadNextLevelScene(renderer, game);
            }
          } else if (gameLevel == 1) {
            game->finalBoss->health -= 30;
            if (game->finalBoss->health <= 0) {
              // enter game win screen
              printf("game won \n");
              fflush(stdout);
              loadGameWinScene(renderer, game);
            }
          }
          updateBossHealthText = 0;          
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case 1: {
      // player attack partial hit
      printf("tactical BS attack!\n");
      fflush(stdout);
      switch(playerAttackHitChance) {
        case 0: {
          // player missed
          game->currentScene[2].text = "You use: 'Tactical BS' - missed!";
          updateFightSceneActionText = 0;
          
          break;
        }
        case 1: {
          // player attack partial hit
          game->currentScene[2].text = "You use: 'Tactical BS' - partial hit!";
          updateFightSceneActionText = 0;
          if (gameLevel == 0) {
            game->firstBoss->health -= 10;
            if (game->firstBoss->health <= 0) {
              // enter next level scene
              printf("next level \n");
              fflush(stdout);
              gameLevel = 1;
              updateBossHealthText = 0;
              updateFightSceneActionText = 0;
              loadNextLevelScene(renderer, game);
            }
          } else if (gameLevel == 1) {
            game->finalBoss->health -= 10;
            if (game->finalBoss->health <= 0) {
              // enter game win screen
              printf("game won \n");
              fflush(stdout);
              loadGameWinScene(renderer, game);
            }
          }
          updateBossHealthText = 0;          
          break;
        }
        case 2: {
          // player hit
          game->currentScene[2].text = "You use: 'Tactical BS' - direct hit!";
          updateFightSceneActionText = 0;
          if (gameLevel == 0) {
            game->firstBoss->health -= 20;
            if (game->firstBoss->health <= 0) {
              // enter next level scene
              printf("next level \n");
              fflush(stdout);
              updateBossHealthText = 0;
              updateFightSceneActionText = 0;
              gameLevel = 1;
              loadNextLevelScene(renderer, game);
            }
          } else if (gameLevel == 1) {
            game->finalBoss->health -= 20;
            if (game->finalBoss->health <= 0) {
              // enter game win screen
              printf("game won \n");
              fflush(stdout);
              loadGameWinScene(renderer, game);
            }
          }
          updateBossHealthText = 0;          
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    default: {
      break;
    }
  }
  game->player->hasAttacked = 1;
}

// function definitions
void handleInput(SDL_Renderer* renderer,Game* game, int *loop) {
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
        handleMouseDown(renderer,game);                        
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
    if (game->gameMenu) {
      destroyGameMenu(game->gameMenu);
    }
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
  inventory->reportAmmo = 0;
  inventory->generatedReports = 0;
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
  player->hasAttacked = 0;
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
  boss->isEnabled = 0;

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
  npc->isEnabled = 0;

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
  item->isEnabled = 0;

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

void updateFightSceneStatusText (SDL_Renderer *renderer, TextRect* textRect, char* newText, SDL_Color color) {
  if (textRect->texture) {
    SDL_DestroyTexture(textRect->texture);
  }
  SDL_Surface* surface = TTF_RenderText_Solid(textRect->font, newText, color);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  
  textRect->texture = texture;
}

void updateTextRect (SDL_Renderer* renderer, TextRect* textRect, int nextVal, SDL_Color color) {
  char buffer[2048];

  if (textRect->texture) {
    SDL_DestroyTexture(textRect->texture);
  }
  sprintf(buffer, textRect->text, nextVal);
  SDL_Surface* surface = TTF_RenderText_Solid(textRect->font, buffer, color);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  
  textRect->texture = texture;
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

void loadNextLevelScene(SDL_Renderer* renderer,Game* game) {
  SDL_SetRenderDrawColor(renderer,205,205,255,255);
  TextRect* titleTextRect = newTextRect(
    renderer,
    CENTER_WIDTH - 100,
    CENTER_HEIGHT - 150,
    200,
    100, 
    100,
    "../assets/OpenSans-Bold.ttf",
    "NEXT LEVEL",
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
  game->currentSceneName = "nextLevel";
  game->updateFunc = &updateNextLevelScene;
  game->renderFunc = &renderNextLevelScene;

  TextRect* titleSceneRects[2] = {titleTextRect, subTitleTextRect};
  if (game->currentScene) {
    destroyCurrentScene(game->currentScene, game->numSceneRects);
  }
  game->numSceneRects = 2;
  game->currentScene = newCurrentScene(2);

  for (int i = 0; i < 2; ++i) {
    game->currentScene[i] = *titleSceneRects[i];
    printf("test again: %s \n", game->currentScene[i].text);
  }
}

void loadGameWinScene(SDL_Renderer* renderer,Game* game) {
  SDL_SetRenderDrawColor(renderer,205,205,255,255);
  TextRect* titleTextRect = newTextRect(
    renderer,
    CENTER_WIDTH - 100,
    CENTER_HEIGHT - 150,
    200,
    100, 
    100,
    "../assets/OpenSans-Bold.ttf",
    "GAME WON",
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
    "Press Enter to go back to the Title Screen",
    SHOW_TEXT,
    BLACK
  );
  game->currentSceneName = "gamewin";
  game->numSceneRects = 2;
  game->updateFunc = &updateGameWinScene;
  game->renderFunc = &renderGameWinScene;

  TextRect* titleSceneRects[2] = {titleTextRect, subTitleTextRect};
  if (game->currentScene) {
    destroyCurrentScene(game->currentScene, game->numSceneRects);
  }
  game->currentScene = newCurrentScene(2);

  for (int i = 0; i < 2; ++i) {
    game->currentScene[i] = *titleSceneRects[i];
    printf("test again: %s \n", game->currentScene[i].text);
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
  TextRect* playerHealthText = newTextRect(
    renderer,
    SCREEN_WIDTH * 0.03,
    SCREEN_HEIGHT * 0.92,
    150,
    25, 
    40,
    "../assets/OpenSans-Bold.ttf",
    "Health: %d",
    SHOW_TEXT,
    WHITE
  );
  TextRect* timeScoreText = newTextRect(
    renderer,
    SCREEN_WIDTH * 0.60,
    SCREEN_HEIGHT * 0.01,
    200,
    25, 
    40,
    "../assets/OpenSans-Bold.ttf",
    "Hours \"Worked\": %d/8",
    SHOW_TEXT,
    WHITE
  );
  TextRect* reportEffText = newTextRect(
    renderer,
    SCREEN_WIDTH * 0.03,
    SCREEN_HEIGHT * 0.01,
    200,
    25, 
    40,
    "../assets/OpenSans-Bold.ttf",
    "Report Eff: x%d",
    SHOW_TEXT,
    WHITE
  );
  TextRect* reportAmmoText = newTextRect(
    renderer,
    SCREEN_WIDTH * 0.60,
    SCREEN_HEIGHT * 0.92,
    200,
    25, 
    40,
    "../assets/OpenSans-Bold.ttf",
    "Report Ammo: %d/5",
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
  game->maleOne->x = 385;
  game->maleOne->y = 330;

  game->femaleOne->x = 200;
  game->femaleOne->y = 320;

  game->femaleTwo->x = 540;
  game->femaleTwo->y = 60;

  if (game->firstBoss->health <= 0) {
    // offscreen after level one and not interactable
    gameLevel = 1;
    game->firstBoss->isEnabled = 0;
    game->firstBoss->x = - 100;
    game->firstBoss->y = - 100;
  } else {
    game->firstBoss->x = 60;
    game->firstBoss->y = 370;
  }

  game->finalBoss->x = 200;
  game->finalBoss->y = 220;

  game->player->x = 75;
  game->player->y = 250;

  // set item positions
  game->copier->x = 500;
  game->copier->y = 380;

  game->waterCooler->x = 500;
  game->waterCooler->y = 50;

  game->computer->x = 50;
  game->computer->y = 35;

  if (game->gameMenu) {
    destroyGameMenu(game->gameMenu);
    game->gameMenu = 0;
  }

  game->gameMenu = newGameMenu(renderer, game, 0,0);

  // sprintf(globalTextBuffer, playerHealthText->text ,game->player->health);
  // playerHealthText->text = globalTextBuffer;

  TextRect* playSceneRects[4] = {playerHealthText,timeScoreText,reportEffText,reportAmmoText};

  game->numSceneRects = 4;
  game->currentScene = newCurrentScene(4);

  for (int i = 0; i < 4; ++i) {
    game->currentScene[i] = *playSceneRects[i];
    printf("test again: %s \n", game->currentScene[i].text);
  }
}

void loadFightScene(SDL_Renderer* renderer, Game* game) {
  // TODO: mini game fight scene

  updatePlayerHealthText = 0;

  game->player->sprite->animation = 8;
  game->player->sprite->frame = 0;


  game->player->destRect.w = game->player->srcRect.w * 6;
  game->player->destRect.h = game->player->srcRect.h * 6;

  game->player->x = 10;
  game->player->y = 220;

  if (gameLevel == 0) {
    game->firstBoss->sprite->animation = 10;
    game->firstBoss->sprite->frame = 0;
    game->firstBoss->destRect.w = game->firstBoss->srcRect.w * 2;
    game->firstBoss->destRect.h = game->firstBoss->srcRect.h * 2;
    game->firstBoss->x = 400;
    game->firstBoss->y = 50;
    game->firstBoss->destRect.x = game->firstBoss->x;
    game->firstBoss->destRect.y = game->firstBoss->y;
  } else if (gameLevel == 1) {
    game->finalBoss->sprite->animation = 10;
    game->finalBoss->sprite->frame = 0;
    game->finalBoss->destRect.w = game->finalBoss->srcRect.w * 2;
    game->finalBoss->destRect.h = game->finalBoss->srcRect.h * 2;
    game->finalBoss->x = 400;
    game->finalBoss->y = 50;
    game->finalBoss->destRect.x = game->finalBoss->x;
    game->finalBoss->destRect.y = game->finalBoss->y;
  }
  game->player->destRect.x = game->player->x;
  game->player->destRect.y = game->player->y;

  if (game->gameMenu) {
    destroyGameMenu(game->gameMenu);
    game->gameMenu = 0;
  }

  game->gameMenu = newGameMenu(renderer, game, 1,1);

  SDL_SetRenderDrawColor(renderer,255,255,255,255);

  TextRect* playerHealthText = newTextRect(
    renderer,
    345,
    330,
    150,
    25, 
    40,
    "../assets/OpenSans-Bold.ttf",
    "Health: %d",
    SHOW_TEXT,
    BLACK
  );

  TextRect* bossHealthText = newTextRect(
    renderer,
    245,
    70,
    150,
    25, 
    40,
    "../assets/OpenSans-Bold.ttf",
    "Boss Health: %d",
    SHOW_TEXT,
    BLACK
  );
  TextRect* fightActionUpdateText = newTextRect(
    renderer,
    CENTER_WIDTH - 150,
    CENTER_HEIGHT - 30,
    380,
    60, 
    35,
    "../assets/OpenSans-Bold.ttf",
    "-Begin Fight-",
    SHOW_TEXT,
    BLACK
  );
  if (game->currentScene) {
    destroyCurrentScene(game->currentScene, game->numSceneRects);
  }

  game->currentSceneName = "fight";
  game->updateFunc = &updateFightScene;
  game->renderFunc = &renderFightScene;

  TextRect* fightSceneRects[3] = {playerHealthText, bossHealthText, fightActionUpdateText};

  game->numSceneRects = 3;
  game->currentScene = newCurrentScene(3);

  for (int i = 0; i < 3; ++i) {
    game->currentScene[i] = *fightSceneRects[i];
    printf("test again: %s \n", game->currentScene[i].text);
  }
}

void loadGameOverScene(SDL_Renderer* renderer,Game* game) {
  SDL_SetRenderDrawColor(renderer,205,205,255,255);
  TextRect* titleTextRect = newTextRect(
    renderer,
    CENTER_WIDTH - 100,
    CENTER_HEIGHT - 150,
    200,
    100, 
    100,
    "../assets/OpenSans-Bold.ttf",
    "GAME OVER",
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
    "Press Enter to go back to the Title Screen",
    SHOW_TEXT,
    BLACK
  );
  game->numSceneRects = 2;
  game->currentSceneName = "gameover";
  game->updateFunc = &updateGameOverScene;
  game->renderFunc = &renderGameOverScene;

  TextRect* titleSceneRects[2] = {titleTextRect, subTitleTextRect};
  if (game->currentScene) {
    destroyCurrentScene(game->currentScene, game->numSceneRects);
  }
  game->currentScene = newCurrentScene(2);

  for (int i = 0; i < 2; ++i) {
    game->currentScene[i] = *titleSceneRects[i];
    printf("test again: %s \n", game->currentScene[i].text);
  }
}

// Definitions
void renderText(SDL_Renderer *renderer, SDL_Texture *texture, TTF_Font *font,SDL_Rect rect, char *text) {
  // SDL_RenderFillRect(renderer,&rect);
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
  item->destRect.x = item->x;
  item->destRect.y = item->y;
  if (item->isEnabled) {
    SDL_RenderFillRect(renderer,&item->destRect);
  }
  SDL_RenderCopyEx(renderer, item->texture, &item->srcRect, &item->destRect, 0, NULL, SDL_FLIP_NONE); 
}

void renderPlayScene(SDL_Renderer *renderer, Game* game) {
  // renders:
  // 1. Game Environment - TODO: in progress
  // 2. Player - TODO: in progress
  // 3. NPCs - TODO: in progress
  // 4. Bosses - TODO: in progress

  // 255-255-224
  SDL_SetRenderDrawColor(renderer, 255, 255, 224, 255);

  
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

  if (game->gameMenu->isActive) {
    renderGameMenu(renderer, game);
  }

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

void renderNextLevelScene(SDL_Renderer *renderer, Game* game) {
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

void renderGameWinScene(SDL_Renderer *renderer, Game* game) {
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

void renderGameOverScene(SDL_Renderer *renderer, Game* game) {
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
  game->fightDelay = 700;
  game->currentScene = 0;
  game->renderFunc = 0;
  game->updateFunc = 0;
  game->gameMenu = 0;

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
    handleInput(renderer,game, &loop);
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