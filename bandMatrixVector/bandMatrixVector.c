#include "bandMatrixVector.h"

#include <tmmintrin.h>
#include <malloc.h>


static inline size_t lower(size_t i, size_t kl) {
    return i < kl ? i : kl;
}


static inline size_t upper(size_t i, size_t ku, size_t dim) {
    // in right case dim - ku should be greater then zero
    return i < (dim - ku) ? ku : dim - i - 1;
}


static inline size_t getRowLen(size_t i, size_t dim, size_t ku, size_t kl) {
    return 1 + lower(i, kl) + upper(i, ku, dim);
}


static inline size_t offset(size_t i, size_t kl) {
    return i <= kl ? 0 : i - kl;
}


int32_t* bandMatrixVectorMultiplication(const char** matrix, const char* vector, size_t dim, size_t kl, size_t ku) {
    int32_t* res = (int32_t*)malloc(dim * sizeof(int32_t));
    if ( ! res) {
        perror("Memory error");
        return NULL;
    }

    for (size_t i = 0; i < dim; ++i) {
        res[i] = 0;
        size_t offs = offset(i, kl);
        size_t until = getRowLen(i, dim, ku, kl);
        size_t lenSSE = until - until % 16;
        until += offs;

        __m128i curMatrix = _mm_loadu_si128((__m128i const*)&matrix[i][offs]);
        __m128i curVector = _mm_loadu_si128((__m128i const*)&vector[offs]);
        for (size_t j = 0; j < lenSSE; j += 16) {
            __m128i middle = _mm_maddubs_epi16(curMatrix, curVector);
            int16_t sseRes[8];
            _mm_storeu_si128((__m128i*)sseRes, middle);
            for (size_t k = 0; k < 8; ++k) {
                res[i] += (int16_t)sseRes[k];
            }
            ++curMatrix;
            ++curVector;
        }

        // rest of multiplication
        for (size_t j = lenSSE + offs; j < until; ++j) {
            res[i] += matrix[i][j] * vector[j];
        }
    }
    return res;
}