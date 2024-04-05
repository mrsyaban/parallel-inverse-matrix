#include <iostream>
#include <omp.h>

using namespace std;

int main(void) {
    int n = 0;
    double **mat = NULL;
    

    cin >> n;
    
    // Allocating memory for matrix array
    mat = new double*[2*n];
    #pragma omp parallel for num_threads(8) shared(mat)
    for (int i = 0; i < 2*n; ++i)
    {
        mat[i] = new double[2*n]();
    }
    
    //Inputs the coefficients of the matrix
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            cin >> mat[i][j];
        }
    }
    
    // Initializing Right-hand side to identity matrix
    #pragma omp parallel for num_threads(8) shared(mat)
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < 2*n; ++j)
        {
            if(j == (i+n))
            {
                mat[i][j] = 1;
            }
        }
    }

    double *pivot_row = NULL;

    // Reducing To Diagonal Matrix
    for(int i = 0; i < n; ++i)
    {
        #pragma omp parallel for num_threads(8) shared(mat)
        for(int j = 0; j < n; ++j)
        {
            if(j != i)
            {
                double d = mat[j][i] / mat[i][i];
                #pragma omp parallel for num_threads(8) shared(mat)
                for(int k = 0; k < n*2; ++k)
                {
                    mat[j][k] -= mat[i][k]*d;
                }
            }
        }
    }
    
    // Reducing To Unit Matrix
    #pragma omp parallel for num_threads(8) shared(mat)
    for(int i = 0; i < n; ++i)
    {
        double d = mat[i][i];
        for(int j = 0; j < 2*n; ++j)
        {
            mat[i][j] = mat[i][j]/d;
        }
    }
    
    cout << n << endl;
    for(int i=0; i < n; ++i)
    {
        for(int j = n; j < 2*n; ++j)
        {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
    
    // Deleting the memory allocated
    for (int i = 0; i < n; ++i)
    {
        delete[] mat[i];
    }
    delete[] mat;
    
    return 0;
}
