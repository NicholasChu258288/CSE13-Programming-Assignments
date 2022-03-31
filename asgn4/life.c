#include "universe.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define ROW   0
#define DELAY 50000

void usage() {
    printf("Synopsis\n"
           "	Simulates John Conway's Game of Life.\n"
           "Usage\n"
           "	./life [tsn:i:o:] [-n generations] [-i input] [-o output]\n"
           "Options\n"
           "	-t		Sets the universe to toroidal.\n"
           "	-s		Will not print.\n"
           "	-n generations  Sets the number of generations for the game.\n"
           "	-i input	Sets the input file used for populating the universe.\n"
           "	-o output	Sets the output file where the final universe will be printed.\n"
           "	-H		Prints usage.");
}

int main(int argc, char **argv) {
    int opt = 0;

    //Setting default files
    FILE *in_file = stdin;
    FILE *out_file = stdout;

    //If silences is true the ncurses will not print
    bool silence = false;
    //Generations that the game will take place over
    int generations = 100;
    //If the universe for the game will be torroidal
    bool toroidal = false;
    //Sets rows and cols used to creating the universe
    uint32_t rows;
    uint32_t cols;

    while ((opt = getopt(argc, argv, "tsn:i:o:H")) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': silence = true; break;
        case 'n': generations = atoi(optarg); break;
        case 'i': in_file = fopen(optarg, "r"); break;
        case 'o': out_file = fopen(optarg, "w"); break;
        case 'H': usage(); break;
        default: usage(); break;
        }
    }

    fscanf(in_file, "%" SCNu32 " %" SCNu32 "\n", &rows, &cols);

    //Creating universe A and B
    Universe *A = uv_create(rows, cols, toroidal);
    Universe *B = uv_create(rows, cols, toroidal);
    Universe *tmp = A;

    uv_populate(A, in_file);
    if (silence == false) {
        initscr();
        curs_set(FALSE);
    }

    //Going through the generations
    for (int g = 0; g < generations; g += 1) {
        if (silence == false) {
            clear();
        }

        //Going through each cell and creating the next generation on B
        for (uint32_t r = 0; r < rows; r += 1) {
            for (uint32_t c = 0; c < cols; c += 1) {
                uint32_t neighbors = uv_census(A, r, c);
                //Condition for survival of live cell
                if (uv_get_cell(A, r, c) == true) {
                    if (neighbors == 3 || neighbors == 2) {
                        uv_live_cell(B, r, c);
                        if (silence == false) {
                            mvprintw(r, c, "o");
                        }
                    } else {
                        uv_dead_cell(B, r, c);
                        if (silence == false) {
                            mvprintw(r, c, ".");
                        }
                    }
                }

                //Condition for dead to become live cell
                if (uv_get_cell(A, r, c) == false) {
                    if (neighbors == 3) {
                        uv_live_cell(B, r, c);
                        if (silence == false) {
                            mvprintw(r, c, "o");
                        }
                    } else {
                        uv_dead_cell(B, r, c);
                        if (silence == false) {
                            mvprintw(r, c, ".");
                        }
                    }
                }
            }
        }
        //At this point the generation should be changed to the next
        //Swap A and B
        tmp = A;
        A = B;
        B = tmp;
        //Refreshed and delays the screen if s was not specified
        if (silence == false) {
            refresh();
            usleep(DELAY);
        }
    }
    if (silence == false) {
        endwin();
    }

    uv_print(A, out_file);

    //Cleaning allocated memory
    uv_delete(A);
    uv_delete(B);

    fclose(in_file);
    fclose(out_file);

    return 0;
}
