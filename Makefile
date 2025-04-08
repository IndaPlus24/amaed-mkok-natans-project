# I hate Makefile
# This bullshit refuses to cooperate


EXE = AQoQ.exe

LD = g++

default: main

main: mainMenu.o gameMaster.o input.o
	$(LD) -o $(EXE) main.cpp mainMenu.o gameMaster.o input.o -lraylib -lGL -lm -lpthread -ldl -lrt -lX11