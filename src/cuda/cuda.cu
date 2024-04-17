#include <iostream>
#include <cuda_runtime.h>
using namespace std;


__global__ void reduceToDiagonal(double *mat, int n, int i) {
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    __syncthreads();
    if (j < n) {
        if (j != i) {
            double d = mat[j * 2*n + i] / mat[i * 2*n + i];
            for (int k = 0; k < 2*n; ++k) {
                mat[j * 2*n + k] -= mat[i * 2*n + k] * d;
            }
            __syncthreads();        
        }
    }
}

__global__ void reduceToUnit(double *mat, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    __syncthreads();
    if (i < n) {
        double d = mat[i * 2 * n + i];
        for (int j = 0; j < 2 * n; ++j) {
            mat[i * 2*n + j] = mat[i * 2*n + j] / d;
        }
        __syncthreads();
    }
}

int main() {
    int n;
    double *mat_host, *mat_device;
    int mat_size = 2 * n * n * sizeof(double);

    cin >> n;

    // Allocating memory for matrix host
    mat_host = new double[2 * n * n];

    // Inputs the coefficients of the matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> mat_host[i * 2*n + j];
        }
    }

    // Initializing Right-hand side to identity matrix
    for (int i = 0; i < n; ++i) {
        for (int j = n; j < 2*n; ++j) {
            if (j == (i + n)) {
                mat_host[i * 2*n + j] = 1;
            } else {
                mat_host[i * 2*n + j] = 0;
            }
        }
    }

    // Allocating memory for matrix device
    cudaMalloc((void **)&mat_device, mat_size);

    cudaMemcpy(mat_device, mat_host, mat_size, cudaMemcpyHostToDevice);

    dim3 blockShape = dim3(32);
    dim3 gridShape = dim3(max(1.0, ceil((double) n / (double)blockShape.x)));

    // Reducing To Diagonal Matrix
    for(int i = 0; i < n; ++i)
    {
        reduceToDiagonal<<<gridShape, blockShape>>>(mat_device, n, i);
    }
    
    // Reducing To Unit Matrix
    reduceToUnit<<<gridShape, blockShape>>>(mat_device, n) ;

    cudaMemcpy(mat_host, mat_device, mat_size, cudaMemcpyDeviceToHost);

    cout << n << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = n; j < 2 * n; ++j) {
            cout << mat_host[i * 2 * n + j] << " ";
        }
        cout << endl;
    }

    // Deleting the memory allocated
    delete[] mat_host;
    cudaFree(mat_device);

    return 0;
}
