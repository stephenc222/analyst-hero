
gamemake: ./src/main.c
	clang -g -L /usr/local/lib -lSDL2 -lSDL2_ttf -lSDL2_image ./src/main.c  -o ./bin/game