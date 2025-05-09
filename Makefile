# Perhaps, I have been to harsh on you

EXE = AQoQ.exe

LD = g++

C_ARGS = -Igame/include/ -Igame/include/states/ -Igame/include/entities -Igame/include/dungeon -Igame/include/items -Igame/include/utils -Iengine/include -Iengine/include/graphics -Wall

# The directory for *.o files
O_DIR = bin/

MAIN_SRC = $(O_DIR)main.o $(O_DIR)mainMenu.o $(O_DIR)gameMaster.o $(O_DIR)input.o $(O_DIR)AI.o $(O_DIR)enemies.o $(O_DIR)player.o $(O_DIR)map.o ${O_DIR}entities.o ${O_DIR}spriteSheet.o ${O_DIR}attack.o ${O_DIR}projectiles.o ${O_DIR}weapons.o

default: main

main: $(EXE)

$(EXE): $(MAIN_SRC)
	$(LD) $(C_ARGS) -o $(EXE) $(MAIN_SRC) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

$(O_DIR)main.o: game/src/main.cpp game/include/states/mainMenu.h game/include/states/gameMaster.h 
	$(LD) $(C_ARGS) -c -o $(O_DIR)main.o game/src/main.cpp

$(O_DIR)mainMenu.o: game/src/states/mainMenu.cpp game/include/states/mainMenu.h game/include/states/gameMaster.h engine/include/input.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)mainMenu.o game/src/states/mainMenu.cpp

$(O_DIR)gameMaster.o: game/src/states/gameMaster.cpp game/include/states/gameMaster.h engine/include/input.h game/include/utils/gameData.h engine/include/graphics/spriteSheet.h game/include/entities/playerFunks.h game/include/entities/projectilesFunks.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)gameMaster.o game/src/states/gameMaster.cpp

$(O_DIR)input.o: engine/src/input.cpp engine/include/input.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)input.o engine/src/input.cpp

$(O_DIR)AI.o: game/src/entities/AI.cpp game/include/entities/AI.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)AI.o game/src/entities/AI.cpp

${O_DIR}map.o: game/src/dungeon/map.cpp game/include/dungeon/map.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)map.o game/src/dungeon/map.cpp

${O_DIR}player.o: game/src/entities/player.cpp game/include/entities/playerFunks.h game/include/entities/entityFunks.h game/include/entities/attackFunks.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)player.o game/src/entities/player.cpp

${O_DIR}enemies.o: game/src/entities/enemies.cpp game/include/entities/enemiesFunks.h game/include/entities/AI.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)enemies.o game/src/entities/enemies.cpp

${O_DIR}entities.o: game/src/entities/entities.cpp game/include/entities/entityFunks.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)entities.o game/src/entities/entities.cpp

${O_DIR}spriteSheet.o: engine/src/graphics/spriteSheet.cpp engine/include/graphics/spriteSheet.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)spriteSheet.o engine/src/graphics/spriteSheet.cpp

${O_DIR}attack.o: game/src/entities/attack.cpp game/include/entities/attackFunks.h game/include/entities/entityFunks.h game/include/entities/projectilesFunks.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)attack.o game/src/entities/attack.cpp

${O_DIR}projectiles.o: game/src/entities/projectiles.cpp game/include/entities/projectilesFunks.h game/include/entities/entityFunks.h game/include/entities/attackFunks.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)projectiles.o game/src/entities/projectiles.cpp

${O_DIR}weapons.o: game/src/items/weapons.cpp
	$(LD) $(C_ARGS) -c -o $(O_DIR)weapons.o game/src/items/weapons.cpp


game/include/states/gameMaster.h: game/include/states/gameState.h
game/include/states/mainMenu.h: game/include/states/gameState.h
game/include/entities/enemiesStruct.h: game/include/entities/AI.h game/include/dungeon/map.h
game/include/dungeon/map.h: game/include/dungeon/tile.h
game/include/utils/gameData.h: game/include/entities/playerStruct.h game/include/entities/enemiesStruct.h game/include/entities/projectilesStruct.h game/include/dungeon/map.h
game/include/entities/playerFunks.h: engine/include/input.h game/include/utils/gameData.h
game/include/entities/playerStruct.h: engine/include/graphics/spriteSheet.h
game/include/entities/entityFunks.h: game/include/utils/gameData.h game/include/utils/collisionLayers.h
game/include/entities/enemiesFunks: game/include/utils/gameData.h
game/include/entities/attackFunks.h: game/include/utils/gameData.h game/include/entities/attackStruct.h
game/include/entities/attackStruct.h: game/include/utils/collisionLayers.h
game/include/enemies/projectilesStruct.h: game/include/entities/attackStruct.h
game/include/enemies/projectilesFunks.h: game/include/utils/gameData.h

clear:
	rm $(O_DIR)*.o
	rm $(EXE)

run: main
	./$(EXE)