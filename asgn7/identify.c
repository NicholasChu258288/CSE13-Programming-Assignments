#include "pq.h"
#include "text.h"
#include "ht.h"
#include "bf.h"
#include "salts.h"
#include "speck.h"
#include "parser.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <regex.h>
#include <unistd.h>

void usage() {
    printf("Synopsis\n"
           "	Takes an anonymous infile and uses a data base to find the\n"
           "	most likely authors of the given text through analyzing"
           "	the linguistic styles and diction of texts.\n"
           "Usage\n"
           "	./identify [d:n:k:l:emch]"
           "Options\n"
           "	-d	Path to data base, default is lib.db.\n"
           "	-n	Specify path the noise file, default is noise.txt.\n"
           "	-k	Specify number of key matches, default is 5.\n"
           "	-l	Specify number of nose words to use, default is 100.\n"
           "	-e	Sets distance metric to Euclidean distance, is the default metric.\n"
           "	-m	Sets metric to Manhattan distance.\n"
           "	-c	Sets distacne to cosine distance."
           "	-h	Displays usage.\n");
}

int main(int argc, char **argv) {
    int opt = 0;

    FILE *noise_file = fopen("noise.txt", "r");
    int matches = 5;
    FILE *db = fopen("lib.db", "r");

    Metric m = EUCLIDEAN;

    while ((opt = getopt(argc, argv, "d:n:k:l:emch")) != -1) {
        switch (opt) {
        case 'd':
            fclose(db);
            db = fopen(optarg, "r");
            if (db == NULL) {
                perror("Could not open data base.");
                return -1;
            }
            break;
        case 'n':
            fclose(noise_file);
            noise_file = fopen(optarg, "r");
            if (noise_file == NULL) {
                perror("Could not open noise file");
                return -1;
            }
            break;
        case 'k': matches = atoi(optarg); break;
        case 'l': noiselimit = atoi(optarg); break;
        case 'e': m = EUCLIDEAN; break;
        case 'm': m = MANHATTAN; break;
        case 'c': m = COSINE; break;
        case 'h': usage(); break;
        default: usage(); break;
        }
    }
    //Creating noise text
    Text *noise = text_create(noise_file, NULL);
    //Creating text for input anonymous text got from stdin
    Text *anon_text = text_create(stdin, noise);

    int n = 0; // Number of author/text pairs
    fscanf(db, "%d\n", &n); // Takes it from top line of db file, if using default should be 285

    PriorityQueue *pq = pq_create(n); // Creates priority queue to track authors

    char author[100];
    char path[100];

    for (int r = 0; r < n; r += 1) {
        fgets(author, 100, db);
        // Removing \n from author
        for (int a = 0; a < 100; a += 1) {
            if (author[a] == '\n') {
                author[a] = '\0';
                break;
            }
        }

        fgets(path, 100, db);
        // Removing \n from file
        for (int p = 0; p < 100; p += 1) {
            if (path[p] == '\n') {
                path[p] = '\0';
                break;
            }
        }

        FILE *cmp_file = fopen(path, "r");
        Text *cmp_text = text_create(cmp_file, noise);

        enqueue(pq, author, text_dist(cmp_text, anon_text, m));

        fclose(cmp_file);
        text_delete(&cmp_text);
    }

    char *print_author;
    double dist = 0.0;

    printf("Top %d,", matches);
    if (m == EUCLIDEAN) {
        printf(" metric: Euclidean distance,");
    }
    if (m == MANHATTAN) {
        printf(" metric: Manhattan distance,");
    }
    if (m == COSINE) {
        printf(" metric: Cosine distance");
    }
    printf(" noise limit: %d\n", noiselimit);

    for (int k = 0; k < matches; k += 1) {
        if (dequeue(pq, &print_author, &dist)) {
            printf("%d) %s", k + 1, print_author);
            printf(" [%.15f]\n", dist);
        }
    }

    fclose(noise_file);
    fclose(db);

    pq_delete(&pq);

    text_delete(&noise);
    text_delete(&anon_text);

    return 0;
}
