#include <stdio.h>
#include <malloc.h>

#include "boxFilter.h"
#include "dataReader.h"

#define DEFAULT_INPUT "image.txt"
#define DEFAULT_OUTPUT "res.txt"


static inline unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo) | ( ((unsigned long long)hi) << 32 );
}


int main(int argc, char* argv[]) {

    // parsing
    size_t height = 0;
    size_t width = 0;
    char* inputFileName = NULL;
    char* outputFileName = NULL;
    argvParserBoxFilter(argc, argv, &inputFileName, &outputFileName, DEFAULT_INPUT, DEFAULT_OUTPUT);
    const uchar** img = readImageData(inputFileName, &height, &width);

    if ( ! img) {
        printf("Error while reading image %s\n", inputFileName);
        return 0;
    }


    // main part
    uchar** resStraightForward = get2DArray(height, width);
    uchar** resOpt = get2DArray(height, width);

    unsigned long long straightForwardStart = rdtsc();
    straightForwardBoxFilter(img, resStraightForward, height, width);
    printf("Straight forward: %lld\n", rdtsc() - straightForwardStart);

    unsigned long long optStart = rdtsc();
    optimizedBoxFilter(img, resOpt, height, width);
    printf("Optimized: %lld\n", rdtsc() - optStart);

    FILE* output = fopen(outputFileName, "w");
    if (output) {
        printData((const uchar**)resOpt, height, width, output);
        fclose(output);
    } else {
        perror("Can not open file");
        printf(" %s\n", outputFileName);
    }

    free2DArray((uchar**)img, height);
    free2DArray(resStraightForward, height);
    free2DArray(resOpt, height);
    free(inputFileName);
    free(outputFileName);
    return 0;
}