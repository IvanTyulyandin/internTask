#pragma once

#include <stddef.h>
#include <stdint.h>


int32_t* bandMatrixVectorMultiplication(const char** matrix, const char* vector, size_t dim, size_t ku, size_t kl);
