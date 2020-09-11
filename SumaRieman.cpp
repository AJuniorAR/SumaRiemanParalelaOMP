#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N_RECTANGULOS 10000000
#define HILOS 8
#define XINICIAL 0
#define XFINAL 10
#define VECES 100
#define N 10
#define PROCESADORES 4

int n[N] = {10000, 20000, 50000,100000, 200000, 500000, 750000, 1000000,2500000,5000000};

double dursec[N], durpar[N],resulSec[N],resulPar[N];

double f(double x)
{
    return (x * x * x);
}

int main(int argc, char const *argv[])
{   
    int dim, veces;
    double limite_inferior = XINICIAL;
    double limite_superior = XFINAL;
    double incremento;
    
    printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n","dim","dursec[dim]","durpar[dim]","#pasos","resultSec","resultPar","Aceleracion","Eficiencia");



    for ( dim = 0; dim < N; dim++)
    {

    
    double  resultadoSec = 0.0, resultadoPar = 0.0;
    double inicio = 0.0, duracionS = 0.0, duracionP = 0.0;
    int i;

    incremento = (limite_superior - limite_inferior) / n[dim];

    

    for (veces = 0; veces < VECES; veces++)
    {
        for (i = 0; i < n[dim]; ++i)
        {
            inicio = omp_get_wtime();

            double x = limite_inferior + i * incremento;
            double y = f(x);
            resultadoSec += y * incremento;

            duracionS += (omp_get_wtime() - inicio);
        }
        inicio = omp_get_wtime();

#pragma omp parallel for num_threads(HILOS) private(i) reduction(+ \
                                                                 : resultadoPar)
        for (i = 0; i < n[dim]; ++i)
        {

            double x = limite_inferior + i * incremento;
            double y = f(x);
            resultadoPar += y * incremento;
        }
        
        duracionP += (omp_get_wtime() - inicio);
    }

    dursec[dim] = duracionS/VECES; 
    durpar[dim] = duracionP/VECES; 
    resulSec[dim]=resultadoSec/VECES;
    resulPar[dim]=resultadoPar/VECES;

     printf("%i\t%f\t%f\t%i\t%f\t%f\t%f\t%f\n",dim, dursec[dim], durpar[dim],n[dim],resulSec[dim],resulPar[dim],dursec[dim]/durpar[dim],(dursec[dim]/durpar[dim])/PROCESADORES);
        
    }
    

    

    return 0;
}
