#include <iostream>
#include <cstdlib>
#include <ctime>

int** genRandMatrix(int size, int maxValue) {
    int** matrix = new int*[size];
    
    for (int i = 0; i < size; i++) {
        int rowSize = rand() % 10 + 1;
        matrix[i] = new int[rowSize + 1];
        
        matrix[i][0] = rowSize;
        
        for (int j = 1; j <= rowSize; j++) {
            matrix[i][j] = rand() % maxValue + 1;
        }
    }
    
    return matrix;
}

void printMatrix(int** matrix, int size) {
    std::cout << size << std::endl;
    
    for (int i = 0; i < size; i++) {
        int rowSize = matrix[i][0];
        
        std::cout << rowSize << ": ";
        for (int j = 1; j <= rowSize; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    srand((time(NULL)));

    int size = rand() % 10 + 1;
    int maxValue = 100;
    int** matrix = genRandMatrix(size, maxValue);

    printMatrix(matrix, size);

    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}
