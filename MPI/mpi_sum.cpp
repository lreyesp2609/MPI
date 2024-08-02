#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10 // Tamaño total del arreglo

int main(int argc, char** argv) {
    int size, rank;
    int* data = NULL;
    int* local_data;
    int local_sum = 0, global_sum = 0;

    // Inicializar MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Número de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID del proceso actual

    // Sincronizar todos los procesos antes de imprimir la información sobre los procesos
    MPI_Barrier(MPI_COMM_WORLD);
    // Mostrar el número de procesos y el rango del proceso actual
    printf("Proceso %d de %d\n", rank, size);

    // Inicializar datos solo en el proceso raíz
    if (rank == 0) {
        data = (int*)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            data[i] = i + 1; // Arreglo con números del 1 al 10
        }
    }

    // Cada proceso debe saber cuántos elementos recibirá
    int local_n = N / size;
    local_data = (int*)malloc(local_n * sizeof(int));

    // Dispersar datos entre todos los procesos
    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcular la suma local
    for (int i = 0; i < local_n; i++) {
        local_sum += local_data[i];
    }

    // Reducir todas las sumas locales en la suma global
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Sincronizar todos los procesos antes de imprimir la suma total
    MPI_Barrier(MPI_COMM_WORLD);

    // Solo el proceso raíz imprime el resultado final
    if (rank == 0) {
        printf("La suma total es: %d\n", global_sum);
        free(data);
    }

    free(local_data);

    // Finalizar MPI
    MPI_Finalize();
    return 0;
}
