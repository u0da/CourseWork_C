#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "fft-complex.c"
#include <time.h>
#include <omp.h>

int M (float x, float y)
{
    if((pow(x, 2) + pow(y, 2)) < pow((M_PI / 2), 2))
        return (1);
    else
        return (0);
}

float p_s (float x, float y, float delta_z)
{
    return (0.375 * M_PI * (pow(x, 2) + pow(y, 2)));
}

float p (float x, float y)
{
    return (0.5 * M_PI * (pow(x, 2) + pow(y, 2)));
}


//ВЕДУТСЯ ТЕХНИЧЕСКИЕ РАБОТЫ, НАСЯИНИКА
/*double complex h(float delta_z)
{
    printf("ok/n");
    float x = 0, y = 0;
    printf("ok/n");
    double complex matr[511][511] = {0};
    //fftw_complex *in, *out;
    //fftw_plan p;
    //out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 512*512);

   // fftw_plan plan =  fftw_plan_dft_3d(512, 512, 0, matr, out, FFTW_FORWARD, FFTW_ESTIMATE);
   printf("ok/n");
    

    for(int i = 0; i < 512; i++)
    {
        for(int j = 0; i < 512; j++)
        {
            x = M_PI / 511 * (i - 255);
            y = M_PI / 511 * (j - 255);
            matr[i][j] = (M(x, y) * cexp(I * p_s(x, y, delta_z) + I * p(x, y)));
        }
    }
        Fft_transform(*matr, 512*512, false);
        printf("ok\n");
        //void fftw_execute(const fftw_plan plan);

    return cabs(**matr);
}
*/
//

int main(int argc, char **argv)
{

    clock_t tic = clock();
    float delta_z = 0 * M_PI;
    float x = 0, y = 0;

    //int r = 3, c = 4, i, j, count;
  
    double complex **h = (double complex **)malloc(1024 * sizeof(double complex *));
    double complex **pic = (double complex **)malloc(1024 * sizeof(double complex *));
    double complex **result = (double complex **)malloc(1024 * sizeof(double complex *));

    for (int i = 0; i < 1024; i++)
        h[i] = (double complex *)malloc(1024 * sizeof(double complex));

    for (int i = 0; i < 1024; i++)
        pic[i] = (double complex *)malloc(1024 * sizeof(double complex));

    for (int i = 0; i < 1024; i++)
        result[i] = (double complex *)malloc(1024 * sizeof(double complex));
  
    
    for (int i = 0; i <  1024; i++)
      for (int j = 0; j < 1024; j++)
        h[i][j] = pic[i][j] = result[i][j] = 0.0;

    
         
  
    // for (i = 0; i <  1024; i++)
    //   for (j = 0; j < 1024; j++)
    //      printf("%d ", result[i][j]);
    

    //double complex h[1024][1024] = {{0}}; // раньше было 512х512
    //double complex **h = (complex double**)malloc( 1024 * 1024 * sizeof(complex double* ));
    // double complex **pic = (complex double**)malloc( 1024*1024 * sizeof(complex double** ));
    // double complex **result = (complex double**)malloc( 1024*1024 * sizeof(complex double** ));

    // for (int i = 0; i < 1024; i++)
    // {
    //     for (int j = 0; j < 1024; j++)
    //     {
    //         h[i][j] = 0.0 ;
    //     }
    // }

    // for (int i = 0; i < 1024; i++)
    // {
    //     for (int j = 0; j < 1024; j++)
    //     {
    //         printf("%d", h[i][j]);
    //     }
    // }
    
    
    //  for (int i = 0; i < 1024; i++)
    // {
    //     for (int j = 0; j < 1024; j++)
    //     {
    //         printf("h-%f    pic-%f  res-%f \n", h[i][j], pic[i][j], result[i][j]);
    //     }
    // }
    
    //тут должен быть int потому что в питоне тоже int
    // попробовать тут с 1024

    #pragma omp parallel for private(i,j) 
    #pragma omp shared(x, y) reduction(+: h)
    for(int i = 0; i < 512; i++)
    {
        for(int j = 0; j < 512; j++)
        {
            x = (M_PI / 511) * (i - 255);
            y = (M_PI / 511) * (j - 255);
            //printf("%f %f\n", x, y);
            h[i][j] = M(x, y) * cexp((I * p_s(x, y, delta_z)) + (I * p(x, y)));
        }
    }
    
    Fft_transform(*h, 1024,false);


    #pragma omp parallel for private(i,j) 
    #pragma omp reduction(+: h)
    for(int i = 0; i < 512; i++)
    {
        for(int j = 0; j < 512; j++)
        {
            h[i][j] = cabs(h[i][j]);
        }
    }

    
    FILE *input_pic;
    
    input_pic = fopen("src_000.txt", "r");
    
        printf("in file\n");
        for(int i=0; i < 512; i++)
        {
            for(int j=0; j < 512; j++)
            {
                fscanf(input_pic, "%lf", &pic[i][j]);
                //printf("%f", input_pic);
            }
        }
        fclose(input_pic);

    
    Fft_transform(*pic, 1024, false);

    #pragma omp parallel for private(i,j) 
    #pragma omp shared(pic, h) reduction(+: result)
    for (int i = 0; i < 1024; i++)
    {
        for (int j = 0; j < 1024; j++)
        {
            result[i][j] = pic[i][j] * h[i][j];
            //printf("%f ", result[i][j]);
        }
        
    }


    Fft_transform(*result, 1024, true);

    for(int i = 0; i < 1024; i++)
    {
        for(int j = 0; j < 1024; j++)
        {
            result[i][j] = cabs(result[i][j]);
            
        }
    }

    FILE *fp;
    fp = fopen("srch.txt", "w");
    for (int i = 0; i < 512; i++)
    {
        for(int j = 0; j < 512; j++)
        { 
            fprintf(fp, "%lf ", result[i][j]) ;
        }
        fprintf(fp, "%c", '\n');
    }
    fclose(fp);
    
   
    // for (size_t i = 0; i < 512; i++)
    // {
    //     for (size_t j = 0; j < 512; j++)
    //     {
    //         printf("%f + i%f\n", creal(res[i][j]), cimag(res[i][j]));
    //     }
        
    // }
    free(h);
    free(pic);
    free(result);
    clock_t toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    return (0);
}