#include "io.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <inttypes.h>

uint64_t bytes_read;
uint64_t bytes_written;

static uint8_t code_buffer[BLOCK]; //Used for write_code and flush codes
static int bit_index;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    //Read bytes from an infile, save it to buffer, and returns the number of bytes read
    int r = 0;
    int r_nbytes = 0;
    r = read(infile, buf + bytes_read, 1); //Number of bytes read from call of read
    bytes_read += r; //How bytes were read from a call of read_bytes
    r_nbytes += r; //Tracks number of bytes that were read by a call of read_bytes, not total
    while (r_nbytes < nbytes && r > 0) {
        r = read(infile, buf + bytes_read, 1);
        bytes_read += r;
        r_nbytes += r;
    }
    return r_nbytes;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    //Writes nbytes from a buffer to an outfile and returns number of bytes writtren
    int wrote = 0;
    int wrote_nbytes = 0;
    wrote = write(outfile, buf - bytes_written, 1);
    bytes_written += wrote;
    wrote_nbytes += wrote;
    while (wrote_nbytes < nbytes && wrote > 0) {
        wrote = write(outfile, buf - bytes_written, 1);
        bytes_written += wrote;
        wrote_nbytes += wrote;
    }
    return wrote_nbytes;
}

bool read_bit(int infile, uint8_t *bit) {
    //Not sure if allowed to use only 1 static variable
    //to track the indexs, used seperate indexes for
    //tracking byte ands bits
    static uint8_t read_buf[BLOCK];
    static int bit_index; //Used for tracking bit index
    int r = 0; //Used to keep track of number of bytes read

    //When index is 0 read new bytes to buffer,
    //happens when either when first starting
    //or after finishing reading the existing buffer
    if (bit_index == 0) {
        r = read_bytes(infile, read_buf, BLOCK);
        //When there are no more bits that can be read
        if (r == 0) {
            return false;
        }
    }
    if (read_buf[bit_index / 8] & (0x1 << bit_index % 8)) {
        *bit = 1;
        bit_index += 1;
        return true;
    } else {
        *bit = 0;
        bit_index += 1;
        return true;
    }
    return false;
}

void write_code(int outfile, Code *c) {
    uint8_t bytes = 0;
    uint8_t bit;
    //Pops bits from c and adds them to the buffer
    //while c still has bits and buffer is not full
    while (!code_empty(c) && (bit_index / 8) < BLOCK) {
        //Pops bits and adds them to buffer when able
        code_pop_bit(c, &bit);
        if (bit == 1) {
            //Set bit in code buffer to 1
            code_buffer[bit_index / 8] |= (0x1 << bit_index % 8);
        } else {
            //Set bit in code buffer to 0
            code_buffer[bit_index / 8] &= ~(0x1 << bit_index % 8);
        }
        bit_index += 1;
        bytes = bit_index / 8;
        if (bit_index / 8 >= BLOCK) {
            //Issue with write, buffer does read the correct
            //input from code but does not write to file correctly
            //Supposed to write to oufile, then reset bit_index
            //and code_buffer
            write(outfile, code_buffer, BLOCK);
            bit_index = 0;
            memset(code_buffer, 0, sizeof(code_buffer));
        }
    }
}

void flush_codes(int outfile) {
    //Writes out remaining bytes from buffer that were not already
    //written
    if (bit_index > 0) {
        int nbytes
            = (bit_index + 8 - 1) / 8; //Round up number of bytes needed for bits left to flush
        write(outfile, code_buffer, nbytes);
        bit_index = 0;
        //Clearing buffer after finished
        memset(code_buffer, 0, sizeof(code_buffer));
    }
}
