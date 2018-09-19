#include <string.h>

#include "dataReader.h"
#include "bandMatrixVector.h"

#define DEFAULT_INPUT "bigMatrix.txt"
#define DEFAULT_OUTPUT "stdout"
#define DEFAULT_VECTOR "bigVector.txt"


static inline unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo) | ( ((unsigned long long)hi) << 32 );
}


int main(int argc, char* argv[]) {
    size_t dim;
    size_t height;
    size_t width;
    size_t ku;
    size_t kl;
    char* inputMatrixFileName = NULL;
    char* outputFileName = NULL;
    char* inputVectorFile = NULL;

    argvParserMatrixVector(argc, argv, &inputMatrixFileName, &outputFileName, &inputVectorFile
            , DEFAULT_INPUT, DEFAULT_OUTPUT, DEFAULT_VECTOR);
    const char** matrix = readBandMatrix(inputMatrixFileName, &height, &width, &kl, &ku);
    if ( ! matrix) {
        printf("Error while reading matrix from %s\n", inputMatrixFileName);
    }
    const char* vector = readVector(inputVectorFile, &dim);

    if (height != width) {
        printf("Error, matrix height not equal to width\n");
    }
    if (dim != width) {
        printf("Error, vector dim not equal to matrix width %zu %zu\n", dim, width);
    }

    unsigned long long start = rdtsc();
    int32_t* result = bandMatrixVectorMultiplication(matrix, vector, dim, kl, ku);
    printf("Done, took %lld clock cycles\n", rdtsc() - start);

    if (result) {
        if (strcmp(outputFileName, "stdout") == 0) {
            printVector(result, dim, stdout);
        } else {
            FILE* out = fopen(outputFileName, "w");
            if (out) {
                printVector(result, dim, out);
                fclose(out);
            } else {
                perror(outputFileName);
            }
        }
        free(result);
    } else {
        printf("Error in multiplication\n");
    }

    free2DSignedArray((char**)matrix, height);
    free((char*)vector);
    // not null 100%, set as default at least in argvParserMatrixVector
    free(inputMatrixFileName);
    free(outputFileName);
    free(inputVectorFile);
}