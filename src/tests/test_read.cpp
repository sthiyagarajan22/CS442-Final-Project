#include "src.hpp"
#include "../refs/testReference.hpp"
#include <mpi.h>
#include <unistd.h>   // for sleep
#include <cstdio>
#include <cmath>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc < 2) {
    	fprintf(stderr, "matrix.bin file path not specified\n");
	return 1;
    }
    
    const char* matrixPath = argv[1];

    PMPI_Init(&argc, &argv);

    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int n = 64;
    int size = n * n;

    double* read_ref = readColsRef(matrixPath, n); 
    double* read_test = readCols(matrixPath, n);

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
	    for (int j = 0; j < n; j++) {
                if (fabs(read_ref[i*n + j] - read_test[i*n + j]) > 1e-06) {
                    fprintf(stderr, "Rank %d got incorrect transpose at position (%d, %d)\n", rank, i, j);
                    MPI_Abort(MPI_COMM_WORLD, -1);
	        }
            }
        }
    }
    free(read_ref);
    free(read_test);

    PMPI_Finalize();
    return 0;
}

