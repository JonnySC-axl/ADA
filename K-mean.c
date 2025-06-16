/*
Instituto Politecnico Nacional
ESCOM
Alumno:
Sgundo Cantero Jonathan Axel
Materia:
Analisis y Diseno de Algoritmos
Grupo:
3CM3

Windows: gcc -o Proyecto.exe Proyecto.c

./Proyecto <imagen_entrada.bmp> <imagen_salida.bmp> <num_centroides> <No. de centroides>

*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#pragma pack(1)

typedef struct {
    unsigned short signature;
    unsigned int file_size;
    unsigned int reserved;
    unsigned int offset;
} BMPHeader;

typedef struct {
    unsigned int size;
    int width;
    int height;
    unsigned short planes;
    unsigned short bits_per_pixel;
    unsigned int compression;
    unsigned int image_size;
    int x_pixels_per_m;
    int y_pixels_per_m;
    unsigned int colors_used;
    unsigned int important_colors;
} BMPInfoHeader;

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Pixel;

void free_image(Pixel** image, int height) {
    if (image) {
        for (int i = 0; i < height; i++) {
            free(image[i]);
        }
        free(image);
    }
}

Pixel** read_bmp(const char* filename, BMPHeader* header, BMPInfoHeader* info_header) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    if (fread(header, sizeof(BMPHeader), 1, file) != 1) {
        fprintf(stderr, "Error al leer header BMP\n");
        fclose(file);
        return NULL;
    }

    if (fread(info_header, sizeof(BMPInfoHeader), 1, file) != 1) {
        fprintf(stderr, "Error al leer info header BMP\n");
        fclose(file);
        return NULL;
    }

    if (header->signature != 0x4D42) {
        fclose(file);
        fprintf(stderr, "No es un archivo BMP valido\n");
        return NULL;
    }

    if (info_header->bits_per_pixel != 24) {
        fclose(file);
        fprintf(stderr, "Solo se soportan archivos BMP de 24 bits\n");
        return NULL;
    }

    if (info_header->width <= 0 || info_header->height <= 0) {
        fclose(file);
        fprintf(stderr, "Dimensiones de imagen invalidas\n");
        return NULL;
    }

    if (info_header->compression != 0) {
        fclose(file);
        fprintf(stderr, "No se soporta compresion en archivos BMP\n");
        return NULL;
    }

    Pixel** image = (Pixel**)malloc(abs(info_header->height) * sizeof(Pixel*));
    if (!image) {
        fprintf(stderr, "Error al asignar memoria para filas de imagen\n");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < abs(info_header->height); i++) {
        image[i] = (Pixel*)malloc(info_header->width * sizeof(Pixel));
        if (!image[i]) {
            fprintf(stderr, "Error al asignar memoria para fila %d\n", i);
            free_image(image, i);
            fclose(file);
            return NULL;
        }
    }

    fseek(file, header->offset, SEEK_SET);

    int padding = (4 - (info_header->width * sizeof(Pixel)) % 4) % 4;
    unsigned char pad_buffer[3];

    int height = abs(info_header->height);
    for (int i = height - 1; i >= 0; i--) {
        if (fread(image[i], sizeof(Pixel), info_header->width, file) != (size_t)info_header->width) {
            fprintf(stderr, "Error al leer datos de imagen en fila %d\n", i);
            free_image(image, height);
            fclose(file);
            return NULL;
        }

        if (padding > 0) {
            fread(pad_buffer, 1, padding, file);
        }
    }

    fclose(file);
    return image;
}

int write_bmp(const char* filename, BMPHeader header, BMPInfoHeader info_header, Pixel** image) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Error al abrir el archivo para escritura");
        return 0;
    }

    if (fwrite(&header, sizeof(BMPHeader), 1, file) != 1) {
        fprintf(stderr, "Error al escribir header BMP\n");
        fclose(file);
        return 0;
    }

    if (fwrite(&info_header, sizeof(BMPInfoHeader), 1, file) != 1) {
        fprintf(stderr, "Error al escribir info header BMP\n");
        fclose(file);
        return 0;
    }

    int padding = (4 - (info_header.width * sizeof(Pixel)) % 4) % 4;
    unsigned char pad[3] = {0, 0, 0};

    int height = abs(info_header.height);
    for (int i = height - 1; i >= 0; i--) {
        if (fwrite(image[i], sizeof(Pixel), info_header.width, file) != (size_t)info_header.width) {
            fprintf(stderr, "Error al escribir datos de imagen en fila %d\n", i);
            fclose(file);
            return 0;
        }

        if (padding > 0) {
            fwrite(pad, 1, padding, file);
        }
    }

    fclose(file);
    return 1;
}

double DistanciaPixel(Pixel p1, Pixel p2) {
    double dr = p1.red - p2.red;
    double dg = p1.green - p2.green;
    double db = p1.blue - p2.blue;
    return sqrt(dr * dr + dg * dg + db * db);
}

void CentroidesRand(Pixel** image, int width, int height, Pixel* centroids, int num_centroids) {
    srand(time(NULL));

    for (int k = 0; k < num_centroids; ++k) {
        int x = rand() % width;
        int y = rand() % height;
        centroids[k] = image[y][x];
        printf("Centroide %d en (%d,%d): RGB(%d,%d,%d)\n", k, x, y,
               centroids[k].red, centroids[k].green, centroids[k].blue);
    }
}

void kmeans_bmp(Pixel** image, int width, int height, int max_iterations, Pixel* centroids, int num_centroids) {
    int* clusters = (int*)calloc(width * height, sizeof(int));
    if (!clusters) {
        fprintf(stderr, "Error al asignar memoria para clusters\n");
        return;
    }

    printf("Centroides iniciales:\n");
    for (int k = 0; k < num_centroids; ++k) {
        printf("  Centroide %d: RGB(%d,%d,%d)\n", k, centroids[k].red, centroids[k].green, centroids[k].blue);
    }

    int changed;
    int iteration = 0;

    do {
        changed = 0;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                double min_dist = -1.0;
                int closest_centroid = -1;

                for (int k = 0; k < num_centroids; ++k) {
                    double dist = DistanciaPixel(image[i][j], centroids[k]);
                    if (closest_centroid == -1 || dist < min_dist) {
                        min_dist = dist;
                        closest_centroid = k;
                    }
                }

                if (clusters[i * width + j] != closest_centroid) {
                    clusters[i * width + j] = closest_centroid;
                    changed++;
                }
            }
        }

        long* sum_r = (long*)calloc(num_centroids, sizeof(long));
        long* sum_g = (long*)calloc(num_centroids, sizeof(long));
        long* sum_b = (long*)calloc(num_centroids, sizeof(long));
        int* counts = (int*)calloc(num_centroids, sizeof(int));

        if (!sum_r || !sum_g || !sum_b || !counts) {
            fprintf(stderr, "Error al asignar memoria para sumas de centroides\n");
            free(clusters);
            free(sum_r); free(sum_g); free(sum_b); free(counts); 
            return;
        }

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int cluster_idx = clusters[i * width + j];
                sum_b[cluster_idx] += image[i][j].blue;
                sum_g[cluster_idx] += image[i][j].green;
                sum_r[cluster_idx] += image[i][j].red;
                counts[cluster_idx]++;
            }
        }

        for (int k = 0; k < num_centroids; ++k) {
            if (counts[k] > 0) {
                centroids[k].blue = (unsigned char)round((double)sum_b[k] / counts[k]);
                centroids[k].green = (unsigned char)round((double)sum_g[k] / counts[k]);
                centroids[k].red = (unsigned char)round((double)sum_r[k] / counts[k]);
            }
        }

        free(sum_r);
        free(sum_g);
        free(sum_b);
        free(counts);

        iteration++;
        printf("Iteracion %d: %d pixeles cambiaron de cluster. Pixeles por cluster: ", iteration, changed);
        for(int k=0; k < num_centroids; ++k) {
        }
        printf("\n");

    } while (changed > 0 && iteration < max_iterations);

    printf("Algoritmo convergio despues de %d iteraciones\n", iteration);
    printf("Centroides finales:\n");
    for (int k = 0; k < num_centroids; ++k) {
        printf("  Centroide %d: RGB(%d,%d,%d)\n", k, centroids[k].red, centroids[k].green, centroids[k].blue);
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = centroids[clusters[i * width + j]];
        }
    }

    free(clusters);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Uso: %s <imagen_entrada.bmp> <imagen_salida.bmp> <num_centroides>\n", argv[0]);
        return 1;
    }

    const char* input_filename = argv[1];
    const char* output_filename = argv[2];
    int num_centroids = atoi(argv[3]);

    if (num_centroids <= 0) {
        fprintf(stderr, "El numero de centroides debe ser un entero positivo.\n");
        return 1;
    }

    BMPHeader header;
    BMPInfoHeader info_header;

    printf("Cargando imagen: %s\n", input_filename);
    Pixel** image = read_bmp(input_filename, &header, &info_header);
    if (!image) {
        fprintf(stderr, "Error al cargar la imagen\n");
        return 1;
    }

    int height = abs(info_header.height);
    printf("Imagen cargada exitosamente: %dx%d pixeles\n", info_header.width, height);

    if (info_header.width * height > 10000000) {
        printf("Advertencia: Imagen muy grande, el procesamiento puede ser lento\n");
    }

    Pixel* centroids = (Pixel*)malloc(num_centroids * sizeof(Pixel));
    if (!centroids) {
        fprintf(stderr, "Error al asignar memoria para centroides\n");
        free_image(image, height);
        return 1;
    }

    CentroidesRand(image, info_header.width, height, centroids, num_centroids);

    printf("\nProcesando imagen con K-means (maximo 100 iteraciones, %d centroides)...\n", num_centroids);
    kmeans_bmp(image, info_header.width, height, 100, centroids, num_centroids);

    printf("Guardando imagen resultante: %s\n", output_filename);
    if (!write_bmp(output_filename, header, info_header, image)) {
        fprintf(stderr, "Error al guardar la imagen\n");
        free_image(image, height);
        free(centroids);
        return 1;
    }

    free_image(image, height);
    free(centroids); 

    printf("Proceso completado exitosamente. Imagen guardada en %s\n", output_filename);
    return 0;
}