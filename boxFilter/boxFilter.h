#pragma once

#include <stddef.h>

#define FILTER_SIZE 5

typedef unsigned char uchar;

void straightForwardBoxFilter(const uchar** img, uchar** resImg, size_t height, size_t width);

void optimizedBoxFilter(const uchar** img, uchar** resImg, size_t height, size_t width);
