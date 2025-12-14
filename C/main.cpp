#include "src.hpp"
#include <mpi.h>
#include <time.h>
#include <vector>

double* readCols(const char* filename, int n) {
    
    int rank, numProcs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    MPI_File file;
    MPI_Status status;
    // opening file
    MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);

    // defining datatype for our columns
    MPI_Datatype colMatrix;
    MPI_Type_vector(n, 1, n, MPI_DOUBLE, &colMatrix);
    MPI_Type_commit(&colMatrix);

    double* matrix = (double*) calloc(n * n * sizeof(double), 1);
    double* colbuff = (double*) malloc(n * sizeof(double));
    
    for (int i = rank; i < n; i += numProcs) {
	MPI_Offset disp = (MPI_Offset) i * sizeof(double);
	// setting file view for noncontiguous read
	MPI_File_set_view(file, disp, MPI_DOUBLE, colMatrix, "native", MPI_INFO_NULL);
	// reading from file
	MPI_File_read_all(file, colbuff, n, MPI_DOUBLE, &status);

	for (int j = 0; j < n; j++) {
	    matrix[i*n + j] = colbuff[j];
	}
    }

    // reducing so all processes have the complete matrix
    MPI_Allreduce(MPI_IN_PLACE, matrix, n*n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    free(colbuff);
    MPI_Type_free(&colMatrix);

    // closing file
    MPI_File_close(&file);
  
    return matrix;
}

void writeTranspose(const char* filename, double* matrix, int n) {

    // setup
    int rank, numProcs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    // delete the file if present 
    MPI_File_delete(filename, MPI_INFO_NULL);
    // write buffer to file in designated area
    MPI_File file;
    // open file with specific modes
    MPI_File_open(MPI_COMM_WORLD, (char*)filename, MPI_MODE_CREATE | MPI_MODE_WRONLY,
                  MPI_INFO_NULL, &file);

    // we can assume that procs evenly divide n
    int rowsPerProc = n / numProcs;
    int startRow = rank * rowsPerProc;

    // each value is set
    int bytesElem = 4;
    int bytesRow = n * (bytesElem + 1) + 1; // spaces and '\n'
   
    int procBytes = rowsPerProc * bytesRow + 1;

    // allocate buffer for all rows for proc
    char* buf = (char*)calloc(procBytes, 1);
    char* ptr = buf;

    // iterate through each row in the chuck of matrix allocated to proc
    for (int i = 0; i < rowsPerProc; i++) {
        int globalRow = startRow + i;
        for (int j = 0; j < n; j++) {
            ptr += sprintf(ptr, "%.2f ", matrix[globalRow * n + j]);
        }
        ptr += sprintf(ptr, "\n");
    }

    // compute file offset for this rank
    MPI_Offset offset = startRow * bytesRow;

    // collectively seek and write to location indicated by offset
    MPI_File_write_at(file, offset, buf, procBytes, MPI_CHAR, MPI_STATUS_IGNORE);
    MPI_File_close(&file);

    free(buf);
}

int tutorial_main(int argc, char* argv[]) {
    
    // 1. Initialize
    MPI_Init(&argc, &argv);

    int rank, numProcs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    // 2. Read the matrix by column
    const char* filename = argv[1];
    int n = atoi(argv[2]);

    double* matrix = readCols(filename, n);

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%lf ", matrix[i * n + j]);
            }
        printf("\n");
        }
    }

    // 3. Writing transposed matrix to file
    const char* writeFile = "transpose.txt";
    writeTranspose(writeFile, matrix, n);

    // 4. Finalize
    MPI_Finalize();
    
    return 0;
}
