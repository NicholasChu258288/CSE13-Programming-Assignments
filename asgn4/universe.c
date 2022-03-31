#include "universe.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    //Allocating size in memory for universe
    Universe *u = (Universe *) malloc(sizeof(Universe));

    //Saving parameters into struct
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;

    //Allocating memory for grid, similar to the matrix
    //example in asgn4.pdf
    u->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t r = 0; r < rows; r += 1) {
        u->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }

    //Return universe struct
    return u;
}

void uv_delete(Universe *u) {
    //Frees all rows in the grid
    for (uint32_t r = 0; r < u->rows; r += 1) {
        free(u->grid[r]);
    }

    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = true;
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    u->grid[r][c] = false;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    return u->grid[r][c];
}

bool uv_populate(Universe *u, FILE *infile) {
    //Declaring variables that scan will save to
    uint32_t rows;
    uint32_t cols;

    //Reads the rest of the lines in the file
    //Uses these rows and cols to create live cells
    while (fscanf(infile, "%" SCNu32 " %" SCNu32 "\n", &rows, &cols) != EOF) {

        //This is to check if it is a valid cell
        if ((rows < 0) || (rows > uv_rows(u) - 1) || (cols < 0) || (cols > uv_cols(u) - 1)) {
            return false;
        }

        uv_live_cell(u, rows, cols);
    }

    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t total_rows = uv_rows(u);
    uint32_t total_cols = uv_cols(u);
    uint32_t neighbors = 0;

    //Census of  neighbors when universe if toroidal
    if (u->toroidal == true) {
        uint32_t prev_row = r - 1;
        uint32_t next_row = r + 1;
        uint32_t prev_col = c - 1;
        uint32_t next_col = c + 1;

        //Setting proper previous and next columns and rows if needed
        if (r == 0) {
            prev_row = (r + total_rows - 1) % total_rows;
        }
        if (r + 1 == total_rows) {
            next_row = (r + 1) % total_rows;
        }
        if (c == 0) {
            prev_col = (c + total_cols - 1) % total_cols;
        }
        if (c + 1 == total_cols) {
            next_col = (c + 1) % total_cols;
        }

        //Census of neighbors
        //For row above r, r-1
        if (uv_get_cell(u, prev_row, prev_col)) {
            neighbors += 1;
        }
        if (uv_get_cell(u, prev_row, c)) {
            neighbors += 1;
        }
        if (uv_get_cell(u, prev_row, next_col)) {
            neighbors += 1;
        }
        //For row r
        if (uv_get_cell(u, r, prev_col)) {
            neighbors += 1;
        }
        if (uv_get_cell(u, r, next_col)) {
            neighbors += 1;
        }
        //For row below r, r+1
        if (uv_get_cell(u, next_row, prev_col)) {
            neighbors += 1;
        }
        if (uv_get_cell(u, next_row, c)) {
            neighbors += 1;
        }
        if (uv_get_cell(u, next_row, next_col)) {
            neighbors += 1;
        }
    }

    //Census of neighbors when universe is not torroidal
    if (u->toroidal == false) {
        //Census of the row above r, r-1
        //Checking if (r-1,c-1) exists
        if (r > 0 && c > 0) {
            if (uv_get_cell(u, r - 1, c - 1)) {
                neighbors += 1;
            }
        }
        //Checking if (r-1, c) exists
        if (r > 0) {
            if (uv_get_cell(u, r - 1, c)) {
                neighbors += 1;
            }
        }
        //Checking if (r-1, c+1)
        if (r > 0 && c + 1 < total_cols) {
            if (uv_get_cell(u, r - 1, c + 1)) {
                neighbors += 1;
            }
        }

        //Census of row r
        //Checking if (r,c-1) exists
        if (c > 0) {
            if (uv_get_cell(u, r, c - 1)) {
                neighbors += 1;
            }
        }
        //Checking if (r, c+1) exists
        if (c + 1 < total_cols) {
            if (uv_get_cell(u, r, c + 1)) {
                neighbors += 1;
            }
        }

        //Census for row below r, r+1
        //Checking if (r+1,c-1) exists
        if (r + 1 < total_rows && c > 0) {
            if (uv_get_cell(u, r + 1, c - 1)) {
                neighbors += 1;
            }
        }
        //Checking if (r+1,c) exists
        if (r + 1 < total_rows) {
            if (uv_get_cell(u, r + 1, c)) {
                neighbors += 1;
            }
        }
        //Checking if (r+1,c+1) exists
        if (r + 1 < total_rows && c + 1 < total_cols) {
            if (uv_get_cell(u, r + 1, c + 1)) {
                neighbors += 1;
            }
        }
    }

    return neighbors;
}

void uv_print(Universe *u, FILE *outfile) {
    uint32_t total_rows = uv_rows(u);
    uint32_t total_cols = uv_cols(u);

    //Prints . for dead and o for live
    for (uint32_t r = 0; r < total_rows; r += 1) {
        for (uint32_t c = 0; c < total_cols; c += 1) {
            if (uv_get_cell(u, r, c) == true) {
                fprintf(outfile, "o");
            }
            if (uv_get_cell(u, r, c) == false) {
                fprintf(outfile, ".");
            }
        }
        //Newline when row ends
        fprintf(outfile, "\n");
    }
}
