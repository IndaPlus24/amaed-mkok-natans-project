# Perhaps, I have been to harsh on you

EXE = AQoQ.exe

LD = g++

C_ARGS = -Wall

# The directory for *.o files
O_DIR = bin/

MAIN_SRC = $(O_DIR)main.o $(O_DIR)mainMenu.o $(O_DIR)gameMaster.o $(O_DIR)input.o

default: main

main: $(MAIN_SRC)
	$(LD) $(C_ARGS) -o $(EXE) $(MAIN_SRC) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

$(O_DIR)main.o: main.cpp include/mainMenu.h include/gameMaster.h 
	$(LD) $(C_ARGS) -c -o $(O_DIR)main.o main.cpp

$(O_DIR)mainMenu.o: mainMenu.cpp include/mainMenu.h include/gameMaster.h include/input.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)mainMenu.o mainMenu.cpp

$(O_DIR)gameMaster.o: gameMaster.cpp include/gameMaster.h include/input.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)gameMaster.o gameMaster.cpp

$(O_DIR)input.o: input.cpp include/input.h
	$(LD) $(C_ARGS) -c -o $(O_DIR)input.o input.cpp

clear:
	rm $(O_DIR)*.o
	rm $(EXE)

run: main
	./$(EXE)