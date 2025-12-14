#include "src.hpp"
#include "../refs/testReference.hpp"
#include <mpi.h>
#include <unistd.h>   // for sleep
#include <cstdio>
#include <cmath>
#include <vector>

int main(int argc, char* argv[])
{
    PMPI_Init(&argc, &argv);

    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int n = 64;
    double* matrix = (double*) malloc(n * n * sizeof(double));

    for (int i = 0; i < n * n; i++) {
	matrix[i] = (double) rand() / (double) RAND_MAX;;
    }

    const char* test_out = "transposedTest.txt";
    const char* ref_out = "transposedRef.txt";

    writeTranspose(test_out, matrix, n);
    writeTransposeRef(ref_out, matrix, n);

    MPI_Barrier(MPI_COMM_WORLD);

    // only need one process to compare
    if (rank == 0) {
	FILE* f1 = fopen(test_out, "r");
        FILE* f2 = fopen(ref_out, "r");

        if (!f1 || !f2) {
            fprintf(stderr, "Could not open output files.\n");
	    MPI_Abort(MPI_COMM_WORLD, -1);
        } else {
            double a, b;
            while (fscanf(f1, "%lf", &a) == 1 &&
                   fscanf(f2, "%lf", &b) == 1) {
                if (a != b) {
		    fprintf(stderr, "Rank %d got incorrect element %lf when element should be %lf\n", rank, a, b);
                    MPI_Abort(MPI_COMM_WORLD, -1);
                }
            }
            fclose(f1);
            fclose(f2);
        }
        printf("Write test passed.\n");
    }

    free(matrix);

    PMPI_Finalize();
    return 0;
}
