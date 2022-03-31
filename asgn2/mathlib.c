#include "mathlib.h"

#include <stdio.h>

#define epsilon 1e-14 /* Epsilon macro */

/* Contains the implementation of math library functions from mathlib.h */

/* Finds apprroximation of e^x. */
double Exp(double x) {

    int k = 1;
    double currentTerm = 1.0;
    double previousTerm = 1.0;
    double expsum = 1.0;

    /* abs() can only be used on ints so for doubles so need to use this*/
    /* to simulate it */
    double absx = x;
    if (absx < 0) {
        absx *= -1;
    }

    while (previousTerm > epsilon) {
        currentTerm = previousTerm * absx / k;
        expsum += currentTerm;
        k++;
        previousTerm = currentTerm;
    }

    /* When x is negative*/
    if (x < 0) {
        expsum = 1 / expsum;
    }

    return expsum;
}

/* Finds approximation of sin(x). */
double Sin(double x) {

    int k = 3;
    double currentTerm = x;
    double previousTerm = x;
    double sinsum = currentTerm;
    double sign = -1;

    double abspreviousTerm = previousTerm;
    if (abspreviousTerm < 0) {
        abspreviousTerm *= -1;
    }

    while (abspreviousTerm > epsilon) {
        currentTerm = previousTerm * (x * x) / ((k - 1) * k);
        sinsum += sign * currentTerm;
        previousTerm = currentTerm;
        k += 2;
        sign *= -1;

        abspreviousTerm = previousTerm;
        if (abspreviousTerm < 0) {
            abspreviousTerm *= -1;
        }
    }
    return sinsum;
}

/* Finds approximation for cos(x). */
double Cos(double x) {
    int k = 2;
    double currentTerm = 1.0;
    double previousTerm = 1.0;
    double cossum = currentTerm;
    int sign = -1;

    double abspreviousTerm = previousTerm;
    if (abspreviousTerm < 0) {
        abspreviousTerm *= -1;
    }

    while (abspreviousTerm > epsilon) {
        currentTerm = previousTerm * ((x * x) / ((k - 1) * k));
        cossum += sign * currentTerm;
        previousTerm = currentTerm;
        sign *= -1;

        abspreviousTerm = previousTerm;
        if (abspreviousTerm < 0) {
            abspreviousTerm *= -1;
        }
        k += 2;
    }
    return cossum;
}

/* Finds approximation for sqrt(x) */
double Sqrt(double x) {
    /* Should be a condition to check when x is negative. */
    /* Can't take a square root of a negative.*/

    double factor = 1.0;
    while (x > 1) {
        x = x / 4.0;
        factor = factor * 2.0;
    }

    double xk = 0.0;
    double y = 1.0;

    double absyxk = y - xk;
    if (absyxk < 0) {
        absyxk *= -1;
    }

    while (absyxk > epsilon) {
        xk = y;
        y = 0.5 * (xk + x / xk);

        absyxk = y - xk;
        if (absyxk < 0) {
            absyxk *= -1;
        }
    }

    return y * factor;
}

/* Finds approximation of log(x) */
double Log(double x) {

    double EulersConstant = 2.7182818284590455;
    double factor = 0;
    while (x > EulersConstant) {
        x /= EulersConstant;
        factor += 1.0;
    }

    double xk = 1.0;
    double ex = Exp(xk);

    double absdiff = ex - x;
    if (absdiff < 0) {
        absdiff *= -1;
    }

    while (absdiff > epsilon) {
        xk = xk + x / ex - 1;
        ex = Exp(xk);

        absdiff = ex - x;
        if (absdiff < 0) {
            absdiff *= -1;
        }
    }

    return factor + xk;
}
