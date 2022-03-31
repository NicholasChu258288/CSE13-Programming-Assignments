##Assignment 4: Game of Life

## Short Description
	The program life.c is meant to simulate John Conway's the Game of Life. Command line options allows the user to feed
	the program an input file and using the in the file data create a universe based on its specifications. The number of
	generations that the game will last and whether the universe will be toroidal can also be specified. Once the game is
	over then the program will print the final resulting universe of the game to a specified output file.

## Building
	make

## Running
	./life [-t] [-s] [-n generations] [-i input_file] [-o output_file]

## Cleaning
	make clean

## Error
	./life will produce memory leaks, however this is primarily due to ncurses. When tested on a seperate test harness
	uv_delete() worked as intended in freed all the allocated data without causing any memory leaks. When setting
	./life to silent it will no produce any memory leaks.
