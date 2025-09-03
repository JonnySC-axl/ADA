#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define TAM 10000

int arreglo[TAM];

void generaAle(int *arr,int min, int max) {
    printf("Llena un arreglo con numeros aleatorios entre %d y %d: ", min, max);
  
    for (int i = 0; i < TAM; i++) {

        // Generamos un número aleatorio entre min y max
        int num_ale = rand() % (max - min + 1) + min;
	arreglo[i]=num_ale;
        printf("%d ", num_ale);
    }
}

void take_enter() {
   printf("Presiona enter para detener el contador \n");
   while(1) {
      if (getchar())
      break;
   }
}
int main() {
   // Calculamos el tiempo que toma ejecutar  take_enter()
   clock_t t;
   t = clock();
   printf("Inicia timer\n");
   generaAle(arreglo,0,TAM);
   printf("Se detuvo el timer e\n");
   t = clock() - t;
   double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculamos el tiempo trascurrido
   printf("Le tomo %f segundos ejecutarse", time_taken);
   return 0;
}

/* Práctica 01 (En equipos de hasta 2 personas)
Programar el algoritmo de ordenamiento por selección
en C y probarlo con archivos de enteros de diferente 
tamaño y en tres condiciones diferentes.
Archivos con las siguientes cantidades de enteros:
 100
 500
 1,500
 5,000
 10,000
 15,000
 50,000
 100,000
 250,000
 500,000
 1,000,000
Y en tres condiciones diferentes:
1)En orden aleatorio
2)En orden inverso
3)En orden
4)Casi ordenados, es decir de un archivo de enteros
ordenados, mover 10 elementos a posiciones diferentes.

Medir el tiempo y graficar los resultados.
*/

