//
//  main.cpp
//  Lab2OpenMPI
//
//  Created by Алексей Лазарев on 10/10/17.
//  Copyright © 2017 Алексей Лазарев. All rights reserved.
//

#include <iostream>
#include "mpi.h"

using namespace std;

int** createMatrix(int rows, int cols) {
    int **matrix = new int *[rows];
    for (int i = 0; i < rows; i++)
        matrix[i] = new int[cols];
    
    return matrix;
}

int** fillMatrix(int rows, int cols, int **matrix, int rank) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rank + i;
        }
    
    return matrix;
}

void displayMatrix(int rows, int cols, int **matrix) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << "===========" << endl;
}

int main(int argc, const char * argv[]) {
    int rank, numberOfProcess, n = 3, m = 4, k = 0, root = 1, *buffer;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
    
    int **matrix = createMatrix(n, m);
    matrix = fillMatrix(n, m, matrix, rank);
    
    if (rank == root) {
        buffer = (int *)malloc(numberOfProcess * m * sizeof(int));
    } else {buffer = NULL;}

    MPI_Gather(matrix[k], m, MPI_INT, buffer, m, MPI_INT, root, MPI_COMM_WORLD);
    
    if (rank == root) {
        matrix = createMatrix(numberOfProcess, m);
        for (int i = 0; i < numberOfProcess; i++) {
            for (int j = 0; j < m; j++) {
                matrix[i][j] = buffer[m * i + j];
            }
        }
        displayMatrix(numberOfProcess, m, matrix);
    }
    
    MPI_Finalize( );
    return 0;
}
