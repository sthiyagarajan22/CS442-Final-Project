#include "src.hpp"

int main(int argc, char* argv[])
{
    PMPI_Init(&argc, &argv);
    
    char* place_argv[] = { (char*)"test_finalize", (char*)"matrix.bin", (char*)"64" };    
    tutorial_main(3, place_argv);

    if (mpi_finalized == false)
    {
        fprintf(stderr, "MPI was not finalized appropriately.  Make sure to call MPI_Finalize.\n");
        return -1;
    }

    PMPI_Finalize();

    return 0;
}
