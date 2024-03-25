// mpicc mpi.c -o mpi

#include <mpi.h>
#include <iostream>
using namespace std;


int main(void) {
    int i = 0, j = 0, k = 0, n = 0;
    double *mat = NULL;
    double d = 0.0;

    MPI_Init(NULL, NULL);

    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == 0) {
        // Master process
        cin >> n;
        
        // Allocating memory for matrix array
        mat = new double[2*n*n];
        
        //Inputs the coefficients of the matrix
        for(i = 0; i < n; ++i)
        {
            for(j = 0; j < n; ++j)
            {
                cin >> mat[i*2*n+j];
            }
        }
        
        // Initializing Right-hand side to identity matrix
        for(i = 0; i < n; ++i)
        {
            for(j = 0; j < 2*n; ++j)
            {
                if(j == (i+n))
                {
                    mat[i*2*n+j] = 1;
                } else if (j >= n )
                {
                    mat[i*2*n+j] = 0;
                }
            }
        }

    } 
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    

    if (world_rank == 0)
    {
        cout << n << endl;
        for (i = 0; i < n; ++i)
        {
            for (j = n; j < 2 * n; ++j)
            {
                cout << mat[i*2*n + j] << " ";
            }
            cout << endl;
        }
        delete[] mat;
    }

    if (world_rank == 0)
    {
        delete[] mat;
    }

    MPI_Finalize();
    return 0;
}