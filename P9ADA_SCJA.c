#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nombre;
    int inicio;
    int fin;
    int salon;  
} Clase;

int compararPorInicio(const void *a, const void *b) {
    Clase *claseA = (Clase *)a;
    Clase *claseB = (Clase *)b;
    return claseA->inicio - claseB->inicio;
}

int seSolapan(Clase c1, Clase c2) {
    return (c1.inicio < c2.fin && c2.inicio < c1.fin);
}

int asignarSalones(Clase clases[], int n) {
    qsort(clases, n, sizeof(Clase), compararPorInicio);
    
    clases[0].salon = 0;
    int numSalones = 1;
    
    for (int i = 1; i < n; i++) {
        int salonAsignado = -1;
        
        for (int s = 0; s < numSalones; s++) {
            int compatible = 1;
            
            for (int j = 0; j < i; j++) {
                if (clases[j].salon == s && seSolapan(clases[i], clases[j])) {
                    compatible = 0;
                    break;
                }
            }
            
            if (compatible) {
                salonAsignado = s;
                break;
            }
        }
        
        if (salonAsignado == -1) {
            salonAsignado = numSalones;
            numSalones++;
        }
        
        clases[i].salon = salonAsignado;
    }
    
    return numSalones;
}

void mostrarHorario(Clase clases[], int n, int numSalones) {
    printf("\n HORARIO DE CLASES\n\n");
    
    for (int s = 0; s < numSalones; s++) {
        printf("Salon %d: ", s + 1);
        int primera = 1;
        
        for (int i = 0; i < n; i++) {
            if (clases[i].salon == s) {
                if (!primera) printf(", ");
                printf("%c[%d-%d]", clases[i].nombre, clases[i].inicio, clases[i].fin);
                primera = 0;
            }
        }
        printf("\n");
    }
}

int main() {
    
    Clase clases[] = {
        {'a', 0, 60, -1},     
        {'b', 60, 180, -1},   
        {'c', 0, 90, -1},      
        {'d', 90, 180, -1},    
        {'e', 120, 240, -1},   
        {'f', 180, 240, -1},   
        {'g', 180, 270, -1},   
        {'h', 240, 390, -1},   
        {'i', 300, 390, -1},   
        {'j', 300, 360, -1}    
    };
    
    int n = sizeof(clases) / sizeof(clases[0]);
    
    printf(" PROBLEMA DE ASIGNACION DE SALONES\n");
    printf("Algoritmo: Greedy (ordenar por tiempo de inicio)\n");
    printf("Numero de clases: %d\n", n);
    
    printf("\nClases originales:\n");
    for (int i = 0; i < n; i++) {
        printf("Clase %c: %d - %d\n", clases[i].nombre, clases[i].inicio, clases[i].fin);
    }
    
    int numSalones = asignarSalones(clases, n);
    
    printf("\n Numero minimo de salones necesarios: %d <<<\n", numSalones);
    
    mostrarHorario(clases, n, numSalones);
    
    printf("\n\n INGRESA TUS PROPIAS CLASES\n");
    printf("Numero de clases a programar: ");
    int numClases;
    scanf("%d", &numClases);
    
    Clase *clasesUsuario = (Clase *)malloc(numClases * sizeof(Clase));
    
    for (int i = 0; i < numClases; i++) {
        printf("\nClase %d:\n", i + 1);
        printf("  Nombre (un caracter): ");
        scanf(" %c", &clasesUsuario[i].nombre);
        printf("  Tiempo de inicio (minutos desde 9am): ");
        scanf("%d", &clasesUsuario[i].inicio);
        printf("  Tiempo de fin (minutos desde 9am): ");
        scanf("%d", &clasesUsuario[i].fin);
        clasesUsuario[i].salon = -1;
    }
    
    int salonesUsuario = asignarSalones(clasesUsuario, numClases);
    
    printf("\n Numero minimo de salones necesarios: %d <<<\n", salonesUsuario);
    mostrarHorario(clasesUsuario, numClases, salonesUsuario);
    
    free(clasesUsuario);
    
    return 0;
}