/*
selection.c
V 1.0 Octubre 2024
Equipo : Null
Grupo: 2CM6
Autor: José Alfonso Vergara Gamboa
Editado por:
	Hernández Zamora Valeria
	Lechuga Torres Diego
	Segundo Cantero Jonathan Alexander
	Vergara Gamboa José Alfonso

El programa lee n numeros de la entrada estandar y los coloca en un arreglo dinamico de longs
posteriormente los ordena utilizando el algoritmo de selección.

Compilación:
Windows: gcc -o selection.exe selection.c
Linux: gcc -o selection.out selection.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void selectionSort(long *array, int size);

int main(int argc, char const *argv[])
{
	int n;
	if (argc != 2)
	{
		printf("Número de elementos:");
		scanf("%d", &n);
	}
	else
		n = atoi(argv[1]);
	long *array = (long *)malloc(n * sizeof(long));
	printf("Introduzca los elementos: \n");
	for (int i = 0; i < n; i++)
		scanf("%ld", &array[i]);
	
	clock_t startTime = clock();

	selectionSort(array, n);

	clock_t endTime = clock();

	for (int i = 0; i < n; i++)
		printf("array[%d] = %ld\n", i, array[i]);

	free(array);
	double interval = (double)(endTime - startTime) / CLOCKS_PER_SEC;
	printf("\nTiempo medido: %.10fs.\n", interval);
	return 0;
}
/*
Recibe: long * Referencia del arreglo array, int tamaño del arreglo
Devuelve: void (No retorna valor explicito)
Observaciones: Encuentra el elemento mas pequeño de la lista y lo intercambia con el primero,
despues busca el siguiente minimo y lo intercambia con el segundo y asi sucesivamente. O(n^2)
*/
void selectionSort(long *array, int size)
{
	// Recorre todos los elementos del arreglo menos el ultimo
	for (int i = 0; i < size - 1; i++)
	{
		// Guarda el index del elemento mas pequeño encontrado
		int smallestIndex = i;
		// Recorre desde el elemento mas pequeño encontrado hacia adelante
		for (int j = smallestIndex + 1; j < size; j++)
		{
			// Si encuentra un elemento mas pequeño guarda su index
			if (array[j] < array[smallestIndex])
				smallestIndex = j;
		}
		long temp = array[smallestIndex];
		array[smallestIndex] = array[i];
		array[i] = temp;
	}
}