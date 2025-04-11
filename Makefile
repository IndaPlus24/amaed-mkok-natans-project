# Perhaps, I have been to harsh on you

EXE = AQoQ.exe

LD = g++

C_ARGS = -Igame/include/ -Igame/include/states/ -Igame/include/entities -Igame/include/dungeon -Iengine/include/ -Wall

# The directory for *.o files
O_DIR = bin/

MAIN_SRC = $(O_DIR)main.o $(O_DIR)mainMenu.o $(O_DIR)gameMaster.o $(O_DIR)input.o

default: main

main: $(MAIN_SRC)
	$(LD) $(C_ARGS) -o $(EXE) $(MAIN_SRC) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

$(O_DIR)main.o: game/src/main.cpp game/include/states/mainMenu.h game/include/states/gameMaster.h 
	$(LD) $(C_ARGS) -c -o $(O_DIR)main.o game/src/main.cpp

$(O_DIR)mainMenu.o: game/src/states/mainMenu.cpp game/include/states/mainMenu.h game/include/states/gameMaster.h engine/include/input.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)mainMenu.o game/src/states/mainMenu.cpp

$(O_DIR)gameMaster.o: game/src/states/gameMaster.cpp game/include/states/gameMaster.h engine/include/input.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)gameMaster.o game/src/states/gameMaster.cpp

$(O_DIR)input.o: engine/src/input.cpp engine/include/input.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)input.o engine/src/input.cpp

$(O_DIR)AI.o: game/src/entities/AI.cpp game/include/entities/AI.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)AI.o game/src/entities/AI.cpp

game/include/states/gameMaster.h: game/include/states/gameState.h
game/include/states/mainMenu.h: game/include/states/mainMenu.h
game/include/entities/enemies.h: game/include/entities/AI.h
game/include/dungeon/map.h: game/include/dungeon/tile.h

clear:
	rm $(O_DIR)*.o
	rm $(EXE)

run: main
	./$(EXE)