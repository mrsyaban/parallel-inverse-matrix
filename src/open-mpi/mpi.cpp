// mpicc mpi.c -o mpi

#include <mpi.h>
#include<iostream>
using namespace std;


int main(void) {
    int i = 0, j = 0, k = 0, n = 0;
    double *mat = NULL;
    double d = 0.0;
    int local_num_rows = 0;
    double *pivot_row = NULL;
    double *local_mat = NULL;

    MPI_Init(NULL, NULL);

    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
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
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    

    // Scatter matrix rows to different processes
    local_num_rows = (n/num_proc);
    pivot_row = new double[2*n];
    if (rank == 0) {
        for(k = 0; k < 2*n; ++k)
        {
            pivot_row[k] = mat[k];
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(pivot_row, 2*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    local_mat = new double[(2*n)*(local_num_rows)];
    MPI_Scatter(mat, (2*n)*local_num_rows, MPI_DOUBLE, 
                local_mat, (2*n)*local_num_rows, MPI_DOUBLE, 
                0, MPI_COMM_WORLD);

    // Reducing to Diagonal Matrix
    for(i = 0; i < n; ++i) 
    {
        for(j = 0; j < local_num_rows; ++j)
        {
            if(rank*local_num_rows+j != i)
            {
                d = local_mat[j*2*n + i] / pivot_row[i];
                for(k = 0; k < 2*n; ++k)
                {
                    local_mat[j*2*n+k] -= pivot_row[k]*d;
                }
            } 
        }    

        MPI_Barrier(MPI_COMM_WORLD);
        if ((i+1)/local_num_rows == rank) {
            for(k = 0; k < 2*n; ++k)
            {
                pivot_row[k] = local_mat[((i+1)%local_num_rows)*2*n + k];
            }
        }

        // Broadcast pivot row for next iteration
        MPI_Barrier(MPI_COMM_WORLD);
        if (i!=n-1) {
            MPI_Bcast(pivot_row, 2*n, MPI_DOUBLE, (i+1)/local_num_rows, MPI_COMM_WORLD);
        }
    }

    // Reducing To Unit Matrix
    for (i = 0; i < local_num_rows; ++i)
    {
        d = local_mat[i * 2*n + (rank*local_num_rows+i)];
        for (j = 0; j < 2*n; ++j)
        {
            local_mat[i*2*n + j] = local_mat[i*2*n + j] / d;
        }
    }

    // Gather results from all processes
    double *result_mat;
    if (rank == 0)
    {
        result_mat = new double[2*n*n];
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(local_mat, 2*n * local_num_rows, MPI_DOUBLE, 
               result_mat, 2*n * local_num_rows, MPI_DOUBLE, 
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        cout << n << endl;
        for (i = 0; i < n; ++i)
        {
            for (j = n; j < 2*n; ++j)
            {
               cout << result_mat[i*2*n + j] << " ";
            }
            cout << endl;
        }
        delete[] result_mat;
    }

    delete[] local_mat;
    if (rank == 0)
    {
        delete[] mat;
    }

    MPI_Finalize();
    return 0;
}