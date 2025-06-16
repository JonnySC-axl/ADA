/*
Instituto Politecnico Nacional
ESCOM
Alumno:
Sgundo Cantero Jonathan Axel
Materia:
Analisis y Diseno de Algoritmos
Grupo:
3CM3

El código genera vectores pseudoaleatorios estructurados 
alrededor de centros de clase (no completamente aleatorios), 
implementa el algoritmo KNN para clasificar nuevos puntos basándose 
en los k vecinos más cercanos del conjunto de entrenamiento, 
permitiendo la clasificación entre un número configurable de clases

Ejecucion:
Windows: gcc -o Knn.exe Knn.c 
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    double *caracteristicas;
    int clase;
} PuntoDatos;

typedef struct {
    double distancia;
    int clase;
} DistanciaClase;

/*
Calcula la distancia de forma euclidiana entre 2 puntos en un 
espacio multidimensional
*/
double calcularDistancia(double *p1, double *p2, int num_caracteristicas) {
    double suma = 0.0;
    for (int i = 0; i < num_caracteristicas; i++) {
        suma += pow(p1[i] - p2[i], 2);
    }
    return sqrt(suma);
}

/* 
Definicion de la funcion llamada compararDistancias, usa qsort para
ordenar un arreglo de estructuras DistanciaClase en orden 
ascendente 
*/
int compararDistancias(const void *a, const void *b) {
    DistanciaClase *da = (DistanciaClase *)a;
    DistanciaClase *db = (DistanciaClase *)b;
    return (da->distancia > db->distancia) ? 1 : -1;
}

/*
Implementa el algoritmo KNN para clasificar un nurvo punto 
en una funcion de un conjunto de datos 
Retornando al final el numero de la clase a la que pertenece 
el nuevo punto segun los k vecinos mas cercanos
*/
int clasificarKNN(PuntoDatos *conjunto, int tam_conjunto, 
                double *punto, int num_caracteristicas, 
                int k, int num_clases) {
    
    DistanciaClase *distancias = malloc(tam_conjunto * sizeof(DistanciaClase));
    if (distancias == NULL) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < tam_conjunto; i++) {
        distancias[i].distancia = calcularDistancia(conjunto[i].caracteristicas, punto, num_caracteristicas);
        distancias[i].clase = conjunto[i].clase;
    }
    
    qsort(distancias, tam_conjunto, sizeof(DistanciaClase), compararDistancias);
    
    int *contador_clases = calloc(num_clases, sizeof(int));
    if (contador_clases == NULL) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < k; i++) {
        contador_clases[distancias[i].clase]++;
    }
    
    int clase_predicha = 0;
    int max_contador = 0;
    for (int i = 0; i < num_clases; i++) {
        if (contador_clases[i] > max_contador) {
            max_contador = contador_clases[i];
            clase_predicha = i;
        }
    }
    
    free(distancias);
    free(contador_clases);
    
    return clase_predicha;
}

/*
Genera un conjunto de datos sinteticos simulando agrupaciones

Cada punto pertenece a una clase
Sus caracteristicas estan agrupadas cerca de un vcentro 
*/
PuntoDatos* generarDatosAgrupados(int num_puntos, int num_caracteristicas, int num_clases) {
    PuntoDatos *datos = malloc(num_puntos * sizeof(PuntoDatos));
    if (datos == NULL) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    
    double **centros = malloc(num_clases * sizeof(double *));
    for (int i = 0; i < num_clases; i++) {
        centros[i] = malloc(num_caracteristicas * sizeof(double));
        for (int j = 0; j < num_caracteristicas; j++) {
            centros[i][j] = (double)rand() / RAND_MAX * 10.0;
        }
    }
    
    for (int i = 0; i < num_puntos; i++) {
        datos[i].caracteristicas = malloc(num_caracteristicas * sizeof(double));
        if (datos[i].caracteristicas == NULL) {
            perror("Error al asignar memoria");
            exit(EXIT_FAILURE);
        }
        
        int clase_actual = rand() % num_clases;
        datos[i].clase = clase_actual;
        
        for (int j = 0; j < num_caracteristicas; j++) {
            double variacion = (double)rand() / RAND_MAX * 2.0 - 1.0;
            datos[i].caracteristicas[j] = centros[clase_actual][j] + variacion;
        }
    }
    
    for (int i = 0; i < num_clases; i++) {
        free(centros[i]);
    }
    free(centros);
    
    return datos;
}

/*
Imprime el conjunto de datos generados
Muestra el numero de puntos y caracteristicas
Para cada punto
    -Su indice
    -Sus caracteristicas
    -Su clase
*/
void imprimirDatos(PuntoDatos *datos, int num_puntos, int num_caracteristicas) {
    printf("\nConjunto de datos (%d puntos, %d Caracteristicas):\n", num_puntos, num_caracteristicas);
    printf("Idx | Caracteristicas");
    for (int i = 0; i < 5*(num_caracteristicas-1)-13; i++) printf(" ");
    printf("| Clase\n");
    
    for (int i = 0; i < num_puntos; i++) {
        printf("%3d | ", i);
        for (int j = 0; j < num_caracteristicas; j++) {
            printf("%5.2f ", datos[i].caracteristicas[j]);
        }
        printf("| %2d\n", datos[i].clase);
    }
}

/*
Libera la memoria usada por un conjunto de datos dinamicamente 
*/
void liberarDatos(PuntoDatos *datos, int num_puntos) {
    for (int i = 0; i < num_puntos; i++) {
        free(datos[i].caracteristicas);
    }
    free(datos);
}

/*
Grafica y ,uestra como el algoritmo Knn clasifica puntos 
en el plano cuando solo hay 2 caracteristicas

Verifica que el conjunto de datos que tengan solo 2 caracteristicas
Genera un archivo de entrenamiento con los puntos y sus claases
Calcula la clase para un rejilla de puntos en el espacio 2D
Muesra la grafica en Gnuplot 
(Es necesario tener Gnuplot instalado en el sistema para que 
se muestre la graficacion para ello se puede descargar el programa 
en:
https://sourceforge.net/projects/gnuplot/
Y agregar Gnpult al path del sistema 
*/
void graficarRegionesDecision(PuntoDatos *datos, int num_puntos, 
                            int num_clases, int k) {
    // Solo funciona para 2 Caracteristicas
    if (datos[0].caracteristicas == NULL || datos[0].caracteristicas[1] == 0) {
        printf("La visualizacion de regiones solo está disponible para 2 Caracteristicas.\n");
        return;
    }

    FILE *gp = popen("gnuplot -persistent", "w");
    if (!gp) {
        printf("Error al abrir Gnuplot. Asegurese de tener Gnuplot instalado.\n");
        return;
    }
    
    FILE *datos_entrenamiento = fopen("knn_datos.txt", "w");
    for (int i = 0; i < num_puntos; i++) {
        fprintf(datos_entrenamiento, "%f %f %d\n", 
                datos[i].caracteristicas[0], 
                datos[i].caracteristicas[1], 
                datos[i].clase);
    }
    fclose(datos_entrenamiento);
    
    FILE *regiones = fopen("knn_regiones.txt", "w");
    double paso = 0.1;
    for (double x = 0; x < 10; x += paso) {
        for (double y = 0; y < 10; y += paso) {
            double punto[2] = {x, y};
            int clase = clasificarKNN(datos, num_puntos, punto, 2, k, num_clases);
            fprintf(regiones, "%f %f %d\n", x, y, clase);
        }
        fprintf(regiones, "\n");
    }
    fclose(regiones);
    
    fprintf(gp, "set title 'Regiones de Decision KNN (k=%d)'\n", k);
    fprintf(gp, "set xlabel 'Característica 1'\n");
    fprintf(gp, "set ylabel 'Característica 2'\n");
    fprintf(gp, "set xrange [0:10]\n");
    fprintf(gp, "set yrange [0:10]\n");
    fprintf(gp, "set key outside\n");
    
    // Definir paleta de colores
    fprintf(gp, "set palette defined (");
    for (int i = 0; i < num_clases; i++) {
        fprintf(gp, "%d '", i);
        switch(i % 8) {
            case 0: fprintf(gp, "red"); break;
            case 1: fprintf(gp, "blue"); break;
            case 2: fprintf(gp, "green"); break;
            case 3: fprintf(gp, "purple"); break;
            case 4: fprintf(gp, "orange"); break;
            case 5: fprintf(gp, "cyan"); break;
            case 6: fprintf(gp, "magenta"); break;
            case 7: fprintf(gp, "brown"); break;
        }
        fprintf(gp, "'");
        if (i < num_clases - 1) fprintf(gp, ", ");
    }
    fprintf(gp, ")\n");
    
    fprintf(gp, "plot 'knn_regiones.txt' using 1:2:3 with image title 'Regiones de decision', \\\n");
    fprintf(gp, "'knn_datos.txt' using 1:2:3 with points pt 7 ps 1 lc palette title 'Datos entrenamiento'\n");
    
    pclose(gp);
}

/*
Punto de entrada completa para probar el codigo knn 
Se veran los datos generado s
Punto de prueba aleatorio
La clase predicha paara el punto en cuestion
Grafica en Gnuplot
*/
int main() {
    int num_puntos = 200;          
    int num_caracteristicas = 2;   
    int num_clases = 3;            
    int k = 5;                     
    
    PuntoDatos *datos_entrenamiento = generarDatosAgrupados(num_puntos, num_caracteristicas, num_clases);
    
    imprimirDatos(datos_entrenamiento, (num_puntos < 10) ? num_puntos : 10, num_caracteristicas);
    
    double punto_prueba[num_caracteristicas];
    printf("\nPunto de prueba: ");
    for (int i = 0; i < num_caracteristicas; i++) {
        punto_prueba[i] = (double)rand() / RAND_MAX * 10.0;
        printf("%.2f ", punto_prueba[i]);
    }
    
    int clase_predicha = clasificarKNN(datos_entrenamiento, num_puntos, punto_prueba, num_caracteristicas, k, num_clases);
    printf("\nClase predicha: %d\n", clase_predicha);
    
    FILE *test_point = fopen("knn_test.txt", "w");
    fprintf(test_point, "%f %f %d\n", punto_prueba[0], punto_prueba[1], clase_predicha);
    fclose(test_point);
    
    graficarRegionesDecision(datos_entrenamiento, num_puntos, num_clases, k);
    
    liberarDatos(datos_entrenamiento, num_puntos);
    
    return 0;
}