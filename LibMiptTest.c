#include <stdio.h>

struct Fraction {
    int n;
    int d;
};

struct Fraction sum(struct Fraction a, struct Fraction b);

struct Fraction subtract(struct Fraction a, struct Fraction b);

struct Fraction inverse(struct Fraction a);

struct Fraction multiply(struct Fraction a, struct Fraction b);

struct Fraction divide(struct Fraction a, struct Fraction b);

int gcd(int a, int b);

void swap(int *a, int *b);

void simplify(struct Fraction *t);

struct Fraction intToFraction(int a);

struct Fraction triangleSum(int a, int b, int c); //a*(b+c)/(a+b+c)


int main() {
    printf("Enter integer values of resistance from task (coefficient of R)\n");
    printf("Start with upper-left, go clockwise and finish with central resistance\n");
    printf("(In line, through space)\n");

    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    scanf("%d %d %d %d %d", &a, &b, &c, &d, &e);

    struct Fraction sum1 = triangleSum(b, e, c);
    struct Fraction sum2 = triangleSum(c, e, b);
    struct Fraction sum3 = triangleSum(e, b, c);

    struct Fraction two = intToFraction(2);
    struct Fraction triangleSum = divide(sum(sum1, sum(sum2, sum3)), two); //(sum1+sum2+sum3)/2

    struct Fraction m = subtract(triangleSum, sum2);
    struct Fraction n = subtract(triangleSum, sum3);
    struct Fraction k = subtract(triangleSum, sum1);

    struct Fraction fraction_a = intToFraction(a);
    struct Fraction fraction_d = intToFraction(d);

    struct Fraction resistance = sum(inverse(sum(inverse(sum(fraction_a, m)), inverse(sum(fraction_d, k)))), n);
    //R=(1/(a+m)+1/(d+k))^(-1)+n
    if (resistance.d != 1) {
        printf("R = %d/%d", resistance.n, resistance.d);
    } else {
        printf("R = %d", resistance.n);
    }
    return 0;
}

struct Fraction triangleSum(int a, int b, int c) { //a*(b+c)/(a+b+c)
    struct Fraction fraction;
    fraction.n = a * (b + c);
    fraction.d = a + b + c;
    simplify(&fraction);
    return fraction;
}

struct Fraction sum(struct Fraction a, struct Fraction b) {//a+b
    struct Fraction fraction;
    fraction.n = a.n * b.d + a.d * b.n;
    fraction.d = a.d * b.d;
    simplify(&fraction);
    return fraction;
}

struct Fraction subtract(struct Fraction a, struct Fraction b) {//a-b
    struct Fraction fraction;
    fraction.n = a.n * b.d - a.d * b.n;
    fraction.d = a.d * b.d;
    simplify(&fraction);
    return fraction;
}

struct Fraction inverse(struct Fraction a) { //a/b to b/a
    int t = a.n;
    a.n = a.d;
    a.d = t;
    return a;
}

struct Fraction multiply(struct Fraction a, struct Fraction b) { //a*b
    struct Fraction fraction;
    fraction.n = a.n * b.n;
    fraction.d = a.d * b.d;
    simplify(&fraction);
    return fraction;
}

struct Fraction divide(struct Fraction a, struct Fraction b) { //a/b
    return multiply(a, inverse(b));
}

struct Fraction intToFraction(int a) {// int a to fraction a/1
    struct Fraction fraction;
    fraction.n = a;
    fraction.d = 1;
    return fraction;
}

int gcd(int a, int b) {
    while (b > 0) {
        a %= b;
        swap(&a, &b);
    }
    return a;
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void simplify(struct Fraction *t) { //simplify fraction
    int grcd = gcd(t->n, t->d);
    t->n = t->n / grcd;
    t->d = t->d / grcd;
}
