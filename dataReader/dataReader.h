#pragma once

#include <stddef.h>
#include <malloc.h>
#include <stdint.h>


typedef unsigned char uchar;

const uchar** readImageData(const char* name, size_t* height, size_t* width);

void printData(const uchar** data, size_t height, size_t width, FILE* ptr);

uchar** get2DArray(size_t height, size_t width);

void free2DArray(uchar** data, size_t height);

void argvParserBoxFilter(int argc, char* argv[]
                         , char** inputFile, char** outputFile
                         , const char* defaultInput, const char* defaultOutput);

void argvParserMatrixVector(int argc, char* argv[]
                            , char** inputMatrixFile, char** outputFile, char** inputVectorFile
                            , const char* defaultInput, const char* defaultOutput, const char* defaultVector);

// duplicated code, replace uchar** with char**
const char** readBandMatrix(const char* name, size_t* height, size_t* width, size_t* kl, size_t* ku);

const char* readVector(const char* name, size_t* dim);

void printSignedData(const char** data, size_t height, size_t width, FILE* ptr);

char** get2DSignedArray();

void free2DSignedArray(char** data, size_t height);

void printVector(int32_t* vector, size_t len, FILE* fptr);