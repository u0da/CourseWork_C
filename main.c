#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "fft-complex.h"

int M (int x, int y)
{
    if(pow(x, 2) + pow(y, 2) < pow(M_PI_2, 2))
        return (0);
    else
        return (1);
}

float p_s (int x, int y, float delta_z)
{
    return 0.375 * M_PI * (pow(x, 2) + pow(y, 2));
}

float p (int x, int y)
{
    return 0.5 * M_PI * (pow(x, 2) + pow(y, 2));
}

double complex h(float delta_z)
{
    float x = 0, y = 0;
    double complex matr[511][511];

    for(int i = 0; i < 512; i++)
        for(int j = 0; i < 512; j++)
        {
            x = M_PI / 511 * (i - 255);
            y = M_PI / 511 * (j - 255);
            matr[i][j] = M(x, y) * cexp(I * p_s(x, y, delta_z) + I * p(x, y));
        }
        Fft_transform(matr, 512, false);
    return abs(matr);
}

int main(int argc, char **argv)
{
    printf("hello\n");
    int a = 1;
    
    return (0);
}