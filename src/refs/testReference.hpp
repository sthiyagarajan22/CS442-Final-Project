#ifndef REFS_HPP
#define REFS_HPP

#ifdef __cplusplus
extern "C" {
#endif

double* readColsRef(const char* filename, int n);
void writeTransposeRef(const char* filename, double* matrix, int n);

#ifdef __cplusplus
}
#endif

#endif
