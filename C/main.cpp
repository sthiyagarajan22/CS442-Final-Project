double* readCols(const char* filename, int n) {
    
    int rank, numProcs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Status status;

    // 1. Open file with MPI

    // 2. Define a dataype for the columns of the matrix

    double* matrix = (double*) calloc(n * n * sizeof(double), 1);
    double* colbuff = (double*) malloc(n * sizeof(double));
    
    for (int i = rank; i < n; i += numProcs) {
	// 3. Use datatype and calculate offset to set file view for the process
	
	
	// 4. Read the column 
	

	// read column gets entered into matrix
	for (int j = 0; j < n; j++) {
	    matrix[i*n + j] = colbuff[j];
	}
    }

    // reducing so all processes have the complete matrix
    MPI_Allreduce(MPI_IN_PLACE, matrix, n*n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    free(colbuff);
    MPI_Type_free(&colMatrix);

    // 5. Close the file
  
    return matrix;
}

void writeTranspose(const char* filename, double* matrix, int n) {

    int rank, numProcs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    // 1.1 Delete the file so that previous runs don't interfere
    // 1.2 Create and open the file that the transposed matrix will be written to


    // we can assume that procs evenly divide n
    int rowsPerProc = n / numProcs;
    int startRow = rank * rowsPerProc;

    // each value is set
    int bytesElem = 4; // each is four chars
    int bytesRow = n * (bytesElem + 1) + 1; // spaces and '\n'
   
    int procBytes = rowsPerProc * bytesRow + 1;

    // allocate buffer for all rows for proc
    char* buf = (char*)calloc(procBytes, 1);
    char* ptr = buf;

    // 2. Populate buffer such that it holds all rows that a process will write

    // 3. Compute the file offset for the process

    // 4. Collectively seek and write buffer to location indicated by offset
    
    // 5. Close the file

    free(buf);
}

int tutorial_main(int argc, char* argv[]) {
    return 0;
}
