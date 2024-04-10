#include <iostream>
#include <cuda_runtime.h>


__global__ void reduceToDiagonal(double *mat, int n) {
}

__global__ void reduceToUnit(double *mat, int n) {
}

int main() {
    int n;
    std::cin >> n;

    double *matHost, *matDevice;
    int size = 2 * n * n * sizeof(double);

    matHost = new double[2 * n * n];
    cudaMalloc((void **)&matDevice, size);

    // Inputs the coefficients of the matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> matHost[i * n + j];
        }
    }

    // Initializing Right-hand side to identity matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 2 * n; ++j) {
            if (j == (i + n)) {
                matHost[i * 2 * n + j] = 1;
            } else {
                matHost[i * 2 * n + j] = 0;
            }
        }
    }

    cudaMemcpy(matDevice, matHost, size, cudaMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;

    // Reducing To Diagonal Matrix kernel
    reduceToDiagonal<<<blocksPerGrid, threadsPerBlock>>>(matDevice, n);

    // Reducing To Unit Matrix kernel
    reduceToUnit<<<blocksPerGrid, threadsPerBlock>>>(matDevice, n);

    cudaMemcpy(matHost, matDevice, size, cudaMemcpyDeviceToHost);

    std::cout << n << std::endl;
    for (int i = 0; i < n; ++i) {
        for (int j = n; j < 2 * n; ++j) {
            std::cout << matHost[i * 2 * n + j] << " ";
        }
        std::cout << std::endl;
    }

    delete[] matHost;
    cudaFree(matDevice);

    return 0;
}
