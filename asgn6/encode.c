#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "pq.h"
#include "stack.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

void usage() {
    printf("Synopsis\n"
           "	Encrypts an input file using Huffman encoding and writes it to\n"
           "	an outfile."
           "Usage\n"
           "	./encode [hi:o:v] -h -i infile -o outfile -v\n"
           "Options\n"
           "	-h		help, prints usage\n"
           "       -i infile 	set infile to encrypt from\n"
           "	-o outfile	set outfile to encrypt to\n"
           "	-v		enables verbose mode");
}

int main(int argc, char **argv) {
    int opt = 0;
    int v = 0; // 0 when not verbose and 1 when verbose
    int infile;
    int outfile;
    while ((opt = getopt(argc, argv, "hi:o:v")) != -1) {
        switch (opt) {
        case 'h': usage(); break;
        case 'i': infile = open(optarg, O_RDONLY); break;
        case 'o': outfile = open(optarg, O_WRONLY); break;
        case 'v': v = 1; break;
        default: usage(); break;
        }
    }
    if (infile) {
        close(infile);
    }
    if (outfile) {
        close(outfile);
    }
    return 0;
}
