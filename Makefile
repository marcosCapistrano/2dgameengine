CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs/"
SRC_FILES = ./src/*.cpp \
			./src/game/*.cpp \
			./src/logger/*.cpp \
			./src/ecs/*.cpp \
			./src/assetstore/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -llua5.4
OBJ_NAME = gameengine

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SRC_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME);

run:
	./gameengine

clean:
	rm gameengine