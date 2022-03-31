#include "functions.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage(char *exec) {
    fprintf(stderr,
        "Synopsis\n"
        "	Integrates a specified function over a interval, both defined by command-line "
        "arguements. \n"
        "\n"
        "Usage\n"
        "	%s [abcdefghijn:p:q:H] [-abcdefghij] [-p low] [-q high] [-n partitions]"
        "\n"
        "Options\n"
        "	-abcdefghi	Chosen function from function.h to integrate.\n"
        "	-p low		Sets low end of the interval to integrate.\n"
        "	-q high		Sets high end of the interval to integrate.\n"
        "	-n partitions   Sets upper limit of partitions."
        "	-H		Displays help and usage.",
        exec);
}

double integrate(double (*f)(double), double a, double b, uint32_t n) {
    /* Using composite  Simpson's One Third Rule. */
    double h = (b - a) / n;
    double simpsonsum = f(a) + f(b);
    for (uint32_t j = 1; j < n; j += 1) {
        if (j % 2 == 0) {
            simpsonsum += 2 * f(j * h + a);
        } else {
            simpsonsum += 4 * f(j * h + a);
        }
    }
    simpsonsum *= h / 3;
    return simpsonsum;
}

int main(int argc, char **argv) {
    /* Performs integration over the interval specified on the command-line. */
    int opt = 0;
    /* Defines a pointer for a function depending on command line arguement used */
    double (*cmdlnf)(double);
    int partitions;
    double lowInterval;
    double highInterval;
    char *equation;

    while ((opt = getopt(argc, argv, "abcdefghijn:p:q:H")) != -1) {
        switch (opt) {
        case 'a':
            cmdlnf = &a; /* pointer now points to function a*/
            equation = "sqrt(1 - x^4)";
            break;
        case 'b':
            cmdlnf = &b;
            equation = "1/log(x)";
            break;
        case 'c':
            cmdlnf = &c;
            equation = "e^(-x^2)";
            break;
        case 'd':
            cmdlnf = &d;
            equation = "sin(x^2)";
            break;
        case 'e':
            cmdlnf = &e;
            equation = "cos(x^2)";
            break;
        case 'f':
            cmdlnf = &f;
            equation = "log(log(x))";
            break;
        case 'g':
            cmdlnf = &g;
            equation = "sin(x)/x";
            break;
        case 'h':
            cmdlnf = &h;
            equation = "e^(-x)/x";
            break;
        case 'i':
            cmdlnf = &i;
            equation = "e^(e^x)";
            break;
        case 'j':
            cmdlnf = &j;
            equation = "sqrt(sin^2(x)+cos^2(x))";
            break;
        case 'n':
            /*Takes string arguement and makes it into an int*/
            partitions = atoi(optarg);
            break;
        case 'p':
            /* Takes input string and turns it into a double*/
            lowInterval = strtod(optarg, NULL);
            break;
        case 'q': highInterval = strtod(optarg, NULL); break;
        case 'H': usage(argv[0]); return EXIT_FAILURE;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }
    /* Using the below to check result.*/
    /* double result = integrate(cmdlnf, lowInterval, highInterval, partitions); */
    /* printf("Result: %.15f \n",result); */

    printf("%s,%f,%f,%d\n", equation, lowInterval, highInterval, partitions);

    for (int currentPartition = 2; currentPartition < partitions + 1; currentPartition += 2) {
        printf("%d,%.15f\n", currentPartition,
            integrate(cmdlnf, lowInterval, highInterval, currentPartition));
    }
}
