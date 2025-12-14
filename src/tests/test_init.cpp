#include "src.hpp"

int main(int argc, char* argv[])
{
    PMPI_Init(&argc, &argv);

    // because program requires command line inputs
    char* place_argv[] = { (char*)"./test_finalize", (char*)"matrix.bin", (char*)"64" };    
    tutorial_main(3, place_argv);

    if (mpi_initialized == false)
    {
        fprintf(stderr, "MPI was not initialized or imported.  Make sure to call MPI_Init in C or load mpi4py in Python\n");
        return -1;
    }

    PMPI_Finalize();
    
    return 0;
}
