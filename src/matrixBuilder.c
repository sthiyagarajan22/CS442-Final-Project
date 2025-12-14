#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);
    int total = n * n;
    
    double* matrix = (double*) malloc(total * sizeof(double));

    for (int i = 0; i < total; i++) {
	matrix[i] = (double) rand() / RAND_MAX;
    }

	    
    FILE* file = fopen("matrix.bin", "wb");
    fwrite(matrix, sizeof(double), total, file);

    fclose(file);
    free(matrix);
    return 0;
}
