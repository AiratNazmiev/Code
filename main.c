//! @File main.c
//! Square equation solving program
//! @mainpage
//! - main.c

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <conio.h>
#include <stdlib.h>

#define SE_INF_SOL -1 //case label does not reduce to an integer constant
const int LE_INF_SOL = -1;
const double DBL_EPS = 0.000001;

/*!
 * @brief Is a equal b
 * @param a double
 * @param b double
 * @return 1 if a = b with defined accuracy, else 0
 */

int isEqual(double a, double b);

/*!
 * @brief Is a bigger than b
 * @param a double
 * @param b double
 * @return 1 if a > b with defined accuracy, else 0
 */

int isBigger(double a, double b);

/*!
 * @brief Solution of square equation
 * @param a Coefficient of ax^2+bx+c=0 square equation
 * @param b Coefficient of ax^2+bx+c=0 square equation
 * @param c Coefficient of ax^2+bx+c=0 square equation
 * @param FirstSol Pointer to first solution
 * @param SecondSol Pointer to second solution
 * @return Number of solutions, -1 if there are not any real solution
 */

int squareEquationSol(double a, double b, double c, double *FirstSol, double *SecondSol);

/*!
 * @brief Solution of reduced quadratic equation ax^2+c=0
 * @param a Coefficient of ax^2+c=0
 * @param c Coefficient of ax^2+c=0
 * @param FirstSol Pointer to first solution
 * @param SecondSol Pointer to second solution
 * @return Number of solutions, -1 if there are not any real solution
 */

int quadraticEquationSol(double a, double c, double *FirstSol, double *SecondSol);

/*!
 * @brief Solution of linear equation
 * @param a Coefficient of ax+b linear equation
 * @param b Coefficient of ax+b linear equation
 * @param Solution Pointer to solution
 * @return Number of solutions, -1 if there are not any real solution
 */

int linearEquationSol(double a, double b, double *Solution);

int main() {
    printf("Solution of square equation\n");
    printf("Program by Nazmiev Airat\n");

    double a = 0;
    double b = 0;
    double c = 0;

    do {
        int num = -1;
        do {
            printf("Enter coefficients a, b, c of square equation ax^2+bx+c=0 (in line, through space):\n");
            system("pause");
            num = scanf("%lf %lf %lf", &a, &b, &c);
        } while (num != 3);

        double FirstSol = 0;
        double SecondSol = 0;

        switch (squareEquationSol(a, b, c, &FirstSol, &SecondSol)) {
            case 2:
                assert(!((&FirstSol == NULL) || (&SecondSol == NULL)));
                printf("Square equation has 2 roots: x1 = %.3f ans x2 = %.3f\n", FirstSol, SecondSol);
                break;
            case 1:
                assert(&FirstSol != NULL);
                printf("Square equation has 1 root: x = %.3f\n", FirstSol);
                break;
            case 0:
                printf("Square equation has not any real root\n");
                break;
            case SE_INF_SOL:
                printf("Every real number is the root of this square equation\n");
                break;
            default:
                fprintf(stderr, "NoSuchCase");
                assert(0);
        }

        printf("If you want to solve another equation, print 1, if not, enter any symbol\n");
    } while (getch() == '1');

    return 0;
}

int squareEquationSol(double a, double b, double c, double *FirstSol, double *SecondSol) { //ax^2+bx+c=0
    assert(!((FirstSol == NULL) || (SecondSol == NULL)));
    assert(FirstSol != SecondSol);
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));

    if (isEqual(a, 0)) {
        return linearEquationSol(b, c, FirstSol);
    } else {
        if (isEqual(b, 0)) {
            return quadraticEquationSol(a, c, FirstSol, SecondSol);
        } else {
            if (isEqual(c, 0)) {
                *FirstSol = 0;
                *SecondSol = -b / a;
                return 2;
            } else {
                double diskr = b * b - 4 * a * c;

                if (isBigger(0, diskr)) {
                    return 0;
                } else if (isBigger(diskr, 0)) {
                    double root = sqrt(diskr);

                    *FirstSol = (-b + root) / (2 * a);
                    *SecondSol = (-b - root) / (2 * a);
                    return 2;
                } else {
                    *FirstSol = -b / (2 * a);
                    return 1;
                }
            }
        }
    }
}

int linearEquationSol(double a, double b, double *Solution) { //ax+b=0
    assert(Solution != NULL);
    assert(isfinite(a));
    assert(isfinite(b));

    if (a == 0) {
        if (b == 0) {
            return LE_INF_SOL;
        } else {
            return 0;
        }
    } else {
        *Solution = -b / a;
        return 1;
    }
}

int quadraticEquationSol(double a, double c, double *FirstSol, double *SecondSol) { //ax^2+c=0
    assert(!((FirstSol == NULL) || (SecondSol == NULL)));
    assert(FirstSol != SecondSol);
    assert(isfinite(a));
    assert(isfinite(c));

    if (c == 0) {
        *FirstSol = 0;
        return 1;
    } else if (-c / a > 0) {
        double root = sqrt(-c / a);

        *FirstSol = root;
        *SecondSol = -root;
        return 2;
    } else {
        return 0;
    }
}

int isEqual(double a, double b) {
    if ((a - b < DBL_EPS) || (b - a < DBL_EPS)) {
        return 1;
    } else {
        return 0;
    }
}

int isBigger(double a, double b) {
    if (a - b > DBL_EPS) {
        return 1;
    } else {
        return 0;
    }
}