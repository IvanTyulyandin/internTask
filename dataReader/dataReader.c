#include "dataReader.h"

#include <malloc.h>
#include <string.h>
#include <errno.h>


static inline uchar** read2DArray(FILE* fptr, uchar** data, size_t height, size_t width) {
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            if (fscanf(fptr, "%hhu", &(data[i][j])) < 1) {
                return NULL;
            }
        }
    }
    return data;
}


static inline int readPair(FILE* fptr, size_t* fst, size_t* snd) {
    if (fscanf(fptr, "%zu %zu", fst, snd) < 2) {
        perror("fscanf failed to read size\n");
        return -1;
    }
    return 0;
}


static const uchar** readData(FILE* fptr, size_t height, size_t width, const char* name) {
    uchar** data = get2DArray(height, width);
    data = read2DArray(fptr, data, height, width);
    fclose(fptr);
    if ( ! data) {
        printf("fscanf failed to read data from %s\n", name);
        free2DArray(data, height);
        return NULL;
    } else {
        return (const uchar**)data;
    }
}


// read image
// return uchar** to image if all was good else NULL
// set height, width
const uchar** readImageData(const char* name, size_t* height, size_t* width) {
    FILE* fptr = fopen(name, "r");
    if (fptr) {
        if (readPair(fptr, height, width)) {
            fclose(fptr);
            return NULL;
        }
        return readData(fptr, *height, *width, name);
    } else {
        perror(name);
        return NULL;
    }
}


void printData(const uchar** data, size_t height, size_t width, FILE* ptr) {
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            fprintf(ptr, "%4hhu ", data[i][j]);
        }
        fprintf(ptr, "\n");
    }
}


uchar** get2DArray(size_t height, size_t width) {
    uchar** res = (uchar**)malloc(height * sizeof(*res));
    if (res) {
        for (size_t i = 0; i < height; ++i) {
            res[i] = (uchar*)malloc(width * sizeof(uchar));
            if ( ! res[i]) {
                perror("Memory error");
                // free previous allocated memory
                free2DArray(res, i);
                res = NULL;
                break;
            }
        }
    } else {
        perror("Memory error");
    }
    return res;
}


void free2DArray(uchar** data, size_t height) {
    for (size_t i = 0; i < height; ++i) {
        free(data[i]);
    }
    free(data);
}


// return 1 if set fileName to value, otherwise 0
static inline int findAndSetFileName(char** fileName, const char* param, const char* option
                                     , const char* value, size_t* i)
{
    if (strcmp(param, option) == 0) {
        if ( ! *fileName) {
            free(*fileName); // in case of multiple option definition
        }
        *fileName = (char*)malloc(255 * sizeof(char));
        if (*fileName) {
            strcpy(*fileName, value);
            ++*i;
        } else {
            perror("Memory error");
        }
        return (*fileName != NULL);
    } else {
        return 0;
    }
}


static inline void setDefaultIfNull(char** fileName, const char* defaultValue) {
    if ( ! *fileName) {
        *fileName = (char*)malloc((strlen(defaultValue) + 1) * sizeof(char));
        if ( ! *fileName) {
            perror("Memory error");
        }
        strcpy(*fileName, defaultValue);
    }
}


void argvParserBoxFilter(int argc, char* argv[]
                         , char** inputFileName, char** outputFileName
                         , const char* defaultInput, const char* defaultOutput)
{
    for (size_t i = 1; i < argc; ++i) {
        if (i + 1 != argc) {
            if ( ! findAndSetFileName(inputFileName, argv[i], "-i", argv[i + 1], &i)) {
                if ( ! findAndSetFileName(outputFileName, argv[i], "-o", argv[i + 1], &i)) {
                    printf("Could not recognize option %s\n", argv[i]);
                }
            }
        }
    }
    setDefaultIfNull(inputFileName, defaultInput);
    setDefaultIfNull(outputFileName, defaultOutput);
}


void argvParserMatrixVector(int argc, char* argv[]
                            , char** inputMatrixFile, char** outputFile, char** inputVectorFile
                            , const char* defaultInput, const char* defaultOutput, const char* defaultVector)
{
    for (size_t i = 1; i < argc; ++i) {
        if (i + 1 != argc) {
            if ( ! findAndSetFileName(inputMatrixFile, argv[i], "-i", argv[i + 1], &i)) {
                if ( ! findAndSetFileName(outputFile, argv[i], "-o", argv[i + 1], &i)) {
                    if ( ! findAndSetFileName(inputVectorFile, argv[i], "-v", argv[i + 1], &i)) {
                        printf("Could not recognize option %s\n", argv[i]);
                    }
                }
            }
        }
    }
    setDefaultIfNull(inputMatrixFile, defaultInput);
    setDefaultIfNull(outputFile, defaultOutput);
    setDefaultIfNull(inputVectorFile, defaultVector);
}


char** get2DSignedArray(size_t height, size_t width) {
    char** res = (char**)malloc(height * sizeof(*res));
    if (res) {
        for (size_t i = 0; i < height; ++i) {
            res[i] = (char*)malloc(width * sizeof(char));
            if ( ! res[i]) {
                perror("Memory error");
                // free previous allocated memory
                free2DSignedArray(res, i);
                res = NULL;
                break;
            }
        }
    } else {
        perror("Memory error");
    }
    return res;
}


void free2DSignedArray(char** data, size_t height) {
    for (size_t i = 0; i < height; ++i) {
        free(data[i]);
    }
    free(data);
}


void printSignedData(const char** data, size_t height, size_t width, FILE* ptr) {
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            int toBeCasted = (int)data[i][j];
            fprintf(ptr, "%4d", toBeCasted);
        }
        fprintf(ptr, "\n");
    }
}


static inline char** read2DSignedArray(FILE* fptr, char** data, size_t height, size_t width) {
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            if (fscanf(fptr, " %hhd", &data[i][j]) < 1) {
                return NULL;
            }
        }
    }
    return data;
}


static const char** readSignedData(FILE* fptr, size_t height, size_t width, const char* name) {
    char** data = get2DSignedArray(height, width);
    data = read2DSignedArray(fptr, data, height, width);
    if ( ! data) {
        printf("fscanf failed to read data from %s\n", name);
        free2DSignedArray(data, height);
        return NULL;
    } else {
        return (const char**)data;
    }
}


// read vector from name
// set dim
const char* readVector(const char* name, size_t* dim) {
    FILE* fptr = fopen(name, "r");
    if (fptr) {
        if (fscanf(fptr, "%zu", dim) < 1) {
            printf("fscanf failed to read dim of vector\n");
        }
        char* res = (char*)malloc(*dim * sizeof(char));
        if (res) {
            for (size_t i = 0; i < *dim; ++i) {
                if (fscanf(fptr, " %hhd", &res[i]) < 1) {
                    printf("fscanf failed to read vector data\n");
                    free(res);
                    res = NULL;
                    break;
                }
            }
        }
        fclose(fptr);
        return res;
    } else {
        perror(name);
        return NULL;
    }
}


// read image
// return uchar** to band matrix if all was good else NULL
// set height, width, kl, ku
const char** readBandMatrix(const char* name, size_t* height, size_t* width, size_t* kl, size_t* ku) {
    FILE* fptr = fopen(name, "r");
    if (fptr) {
        if (readPair(fptr, height, width)) {
            printf("Size: %zu, %zu\n", *height, *width);
            fclose(fptr);
            return NULL;
        }

        if (readPair(fptr, kl, ku)) {
            fclose(fptr);
            return NULL;
        }
        const char** res = readSignedData(fptr, *height, *width, name);
        if ( ! res) {
            printf("Error while reading data from %s\n", name);
        }
        fclose(fptr);
        return res;
    } else {
        perror(name);
        return NULL;
    }
}


void printVector(int32_t* vector, size_t len, FILE* fptr) {
    for (size_t i = 0; i < len; ++i) {
        fprintf(fptr, "%ld ", (long)vector[i]);
    }
    fprintf(fptr, "\n");
}