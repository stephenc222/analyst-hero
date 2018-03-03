
gamemake: ./src/main.c
	clang -L /usr/local/lib -lSDL2 -lSDL2_ttf ./src/main.c  -o ./bin/game