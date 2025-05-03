# Perhaps, I have been to harsh on you

EXE = AQoQ.exe

LD = g++

C_ARGS = -Igame/include/ -Igame/include/states/ -Igame/include/entities -Igame/include/dungeon -Igame/include/utils -Iengine/include/ -Wall

# The directory for *.o files
O_DIR = bin/

MAIN_SRC = $(O_DIR)main.o $(O_DIR)mainMenu.o $(O_DIR)gameMaster.o $(O_DIR)input.o $(O_DIR)AI.o $(O_DIR)enemies.o $(O_DIR)player.o $(O_DIR)map.o ${O_DIR}entities.o

default: main

main: $(MAIN_SRC)
	$(LD) $(C_ARGS) -o $(EXE) $(MAIN_SRC) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

$(O_DIR)main.o: game/src/main.cpp game/include/states/mainMenu.h game/include/states/gameMaster.h 
	$(LD) $(C_ARGS) -c -o $(O_DIR)main.o game/src/main.cpp

$(O_DIR)mainMenu.o: game/src/states/mainMenu.cpp game/include/states/mainMenu.h game/include/states/gameMaster.h engine/include/input.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)mainMenu.o game/src/states/mainMenu.cpp

$(O_DIR)gameMaster.o: game/src/states/gameMaster.cpp game/include/states/gameMaster.h engine/include/input.h game/include/utils/gameData.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)gameMaster.o game/src/states/gameMaster.cpp

$(O_DIR)input.o: engine/src/input.cpp engine/include/input.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)input.o engine/src/input.cpp

$(O_DIR)AI.o: game/src/entities/AI.cpp game/include/entities/AI.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)AI.o game/src/entities/AI.cpp

${O_DIR}map.o: game/src/dungeon/map.cpp game/include/dungeon/map.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)map.o game/src/dungeon/map.cpp

${O_DIR}player.o: game/src/entities/player.cpp game/include/entities/playerFunks.h game/include/entities/entityFunks.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)player.o game/src/entities/player.cpp

${O_DIR}enemies.o: game/src/entities/enemies.cpp game/include/entities/enemies.h game/include/entities/AI.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)enemies.o game/src/entities/enemies.cpp

${O_DIR}entities.o: game/src/entities/entities.cpp game/include/entities/entityFunks.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)entities.o game/src/entities/entities.cpp



game/include/states/gameMaster.h: game/include/states/gameState.h
game/include/states/mainMenu.h: game/include/states/gameState.h
game/include/entities/enemies.h: game/include/entities/AI.h game/include/entities/playerStruct.h game/include/dungeon/map.h
game/include/dungeon/map.h: game/include/dungeon/tile.h
game/include/utils/gameData.h: game/include/entities/playerStruct.h game/include/entities/enemies.h game/include/dungeon/map.h
game/include/entities/playerFunks.h: engine/include/input.h game/include/utils/gameData.h
game/include/entities/entityFunks.h: game/include/utils/gameData.h

clear:
	rm $(O_DIR)*.o
	rm $(EXE)

run: main
	./$(EXE)