#!/bin/bash
CC_ERR="\x1b[31;4m"
CC_OK="\x1b[32;1m"
CC_RESET="\x1b[0m"

rm -rf ./dist

# prepare build directory
echo -e "${CC_OK}Preparing build directory...${CC_RESET}"
mkdir -p ./dist
# mkdir -p ./dist/i686/bin
# mkdir -p ./dist/x86_64/bin
mkdir -p ./dist/osx/bin

# copy assets
echo -e "${CC_OK}Copying assets...${CC_RESET}"
# cp -R ./assets ./dist/i686
# cp -R ./assets ./dist/x86_64
cp -R ./assets ./dist/osx

# copy docs
echo -e "${CC_OK}Copying docs...${CC_RESET}"
# cp -R ./docs/*.md ./dist/i686/
# cp -R ./docs/*.md ./dist/x86_64/
cp -R ./docs/*.md ./dist/osx/

# copy SDL binaries
# echo -e "${CC_OK}Copying SDL binaries...${CC_RESET}"
# cp -R ./external/SDL2/32/bin/*.dll ./dist/i686/bin
# cp -R ./external/SDL2/64/bin/*.dll ./dist/x86_64/bin

# function build_32_bit_executable {
#   if [ `which i686-w64-mingw32-gcc` != "" ]; then
#     echo -e "${CC_OK}MingW is installed. Excellent${CC_RESET}"
#   else
#     echo -e "${CC_ERR}MingW not found. Installing with brew${CC_RESET}"
#     brew install mingw-w64
#   fi

#   INCP="-I/usr/local/Cellar/mingw-w64/5.0.3_2/toolchain-i686/i686-w64-mingw32/include -I./external/SDL2/32/include"
#   LIBP="-L/usr/local/Cellar/mingw-w64/5.0.3_2/toolchain-i686/i686-w64-mingw32/lib -L./external/SDL2/32/lib"
#   FLAGS="-w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf"
#   # INCP="-I/usr/local/Cellar/mingw-w64/5.0.3_2/toolchain-i686/i686-w64-mingw32/include"
#   # LIBP="-L/usr/local/Cellar/mingw-w64/5.0.3_2/toolchain-i686/i686-w64-mingw32/lib"
#   # FLAGS="-w -Wl,-subsystem,windows -lmingw32"
#   EXE="dist/i686/bin/game_i686.exe"
#   SRC="src/main.c"
#   i686-w64-mingw32-gcc $SRC $INCP $LIBP $FLAGS -o $EXE
# }

# function build_64_bit_executable {
#   if [ `which i686-w64-mingw32-gcc` != "" ]; then
#     echo -e "${CC_OK}MingW is installed. Excellent${CC_RESET}"
#   else
#     echo -e "${CC_ERR}MingW not found. Installing with brew${CC_RESET}"
#     brew install mingw-w64
#   fi
#   INCP="-I/usr/local/Cellar/mingw-w64/5.0.3_2/toolchain-x86_64/x86_64-w64-mingw32/include -I./external/SDL2/64/include"
#   LIBP="-L/usr/local/Cellar/mingw-w64/5.0.3_2/toolchain-x86_64/x86_64-w64-mingw32/lib -L./external/SDL2/64/lib"
#   FLAGS="-w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf"
#   # INCP="-I/usr/local/Cellar/mingw-w64/5.0.3_2/toolchain-x86_64/x86_64-w64-mingw32/include"
#   # LIBP="-L/usr/local/Cellar/mingw-w64/5.0.3_2/toolchain-x86_64/x86_64-w64-mingw32/lib"
#   # FLAGS="-w -Wl,-subsystem,windows -lmingw32"
#   EXE="dist/x86_64/bin/game_x86_64.exe"
#   SRC="src/main.c"
#   x86_64-w64-mingw32-gcc $SRC $INCP $LIBP $FLAGS -o $EXE
# }

# assumes SDL2 for OS X is installed on your system locally
# easy way to install sdl2 libraries:
# brew install sdl2{,_gfx,_image,_mixer,_net,_ttf}
function build_osx_executable {
  LIBP="$(pkg-config SDL2 SDL2_ttf SDL2_image --libs) -Wl,-headerpad_max_install_names"
  FLAGS="$(pkg-config SDL2 SDL2_ttf SDL2_image --cflags) -O0 -MMD -MP -g"
  EXE="dist/osx/bin/game"
  SRC="src/main.c"
  cc $SRC $FLAGS $LIBP -o $EXE
}

# read -p "Build for OSX? " -n 1 -r
# echo
# if [[ $REPLY =~ ^[Yy]$ ]]
# then
#   echo -e "${CC_OK}Building OSX binary...${CC_RESET}"
#   build_osx_executable
# fi

# read -p "Build for Windows 32-bit? " -n 1 -r
# echo
# if [[ $REPLY =~ ^[Yy]$ ]]
# then
#   echo -e "${CC_OK}Building 32-bit binary...${CC_RESET}"
#   build_32_bit_executable
# fi

# read -p "Build for Windows 64-bit? " -n 1 -r
# echo
# if [[ $REPLY =~ ^[Yy]$ ]]
# then
#   echo -e "${CC_OK}Building 64-bit binary...${CC_RESET}"
#   build_64_bit_executable
# fi

# and we are done
# echo -e "${CC_OK}Building Finished.${CC_RESET}"

echo -e "${CC_OK}Building OSX binary...${CC_RESET}"
build_osx_executable
echo -e "${CC_OK}Building Finished.${CC_RESET}"
