
gamemake: ./src/main.c
	clang ./src/main.c -MMD -MP -g -L/usr/local/lib -lSDL2 -lSDL2_ttf -lSDL2_image  -o ./bin/game