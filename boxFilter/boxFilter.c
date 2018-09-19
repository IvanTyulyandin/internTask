#include "boxFilter.h"

#include <stdio.h>

static inline void blurOnePixel(const uchar** img, uchar** resImg
                                , int x, int y
                                , size_t height, size_t width
                                , uchar halfSize)
{
    float res = 0.0;
    for (int i = x - halfSize; i <= x + halfSize; ++i) {
        for (int j = y - halfSize; j <= y + halfSize; ++j) {
            res += (float)img[i][j]  / (FILTER_SIZE * FILTER_SIZE);
        }
    }
    resImg[x][y] = (uchar)res;
}


static inline void blurPixelNearBorder(const uchar** img, uchar** resImg
                                       , int x, int y
                                       , size_t height, size_t width
                                       , uchar halfSize)
{
    // brute force counting pixels that are in borders
    unsigned int numOfPixels = 0;
    for (int i = x - halfSize; i <= x + halfSize; ++i) {
        if (i >= 0 && i < height) {
            for (int j = y - halfSize; j <= y + halfSize; ++j) {
                if (j >= 0 && j < width) {
                    ++numOfPixels;
                }
            }
        }
    }

    float res = 0.0;
    for (int i = x - halfSize; i <= x + halfSize; ++i) {
        if (i >= 0 && i < height) {
            for (int j = y - halfSize; j <= y + halfSize; ++j) {
                if (j >= 0 && j < width) {
                    res += (float)img[i][j] / numOfPixels;
                }
            }
        }
    }

    resImg[x][y] = (uchar)(res);
}


static inline void applyToBorder(const uchar** img, uchar** resImg
                                 , size_t height, size_t width
                                 , uchar halfSize, size_t endRow)
{
    // borders
    // top and bottom rows
    for (int i = 0; i < halfSize; ++i) {
        for (int j = 0; j < width; ++j) {
            blurPixelNearBorder(img, resImg, i, j, height, width, halfSize);
            blurPixelNearBorder(img, resImg, (int)height - i - 1, j, height, width, halfSize);
        }
    }
    // left and right sides
    for (int i = halfSize; i < endRow; ++i) {
        for (int j = 0; j < halfSize; ++j) {
            blurPixelNearBorder(img, resImg, i, j, height, width, halfSize);
            blurPixelNearBorder(img, resImg, i, (int)width - j - 1, height, width, halfSize);
        }
    }
}



void straightForwardBoxFilter(const uchar** img, uchar** resImg, size_t height, size_t width) {
    uchar halfSize = FILTER_SIZE / 2;
    size_t endRow = height - halfSize;
    size_t endCol = width - halfSize;
    for (int i = halfSize; i < endRow; ++i) {
        for (int j = halfSize; j < endCol; ++j) {
            blurOnePixel(img, resImg, i, j, height, width, halfSize);
        }
    }
    applyToBorder(img, resImg, height, width, halfSize, endRow);
}


static inline float columnSum(const uchar** img, size_t x, size_t y, uchar halfSize) {
    float res = 0.0;
    size_t endRow = x + halfSize;
    for (size_t i = x - halfSize; i <= endRow; ++i) {
        res += (float)img[i][y] / (FILTER_SIZE * FILTER_SIZE);
    }
    return res;
}


void optimizedBoxFilter(const uchar** img, uchar** resImg, size_t height, size_t width) {
    // process data that fits borders
    const uchar halfSize = FILTER_SIZE / 2;
    const size_t endRow = height - halfSize;
    const size_t endCol = width - halfSize;
    float colSums[FILTER_SIZE];

    for (size_t i = halfSize; i < endRow; ++i) {
        float allColsInKernelSum = 0;
        for (size_t j = 0; j < FILTER_SIZE; ++j) {
            colSums[j] = columnSum(img, i, j, halfSize);
            allColsInKernelSum += colSums[j];
        }
        uchar curPos = 0; // position of the "left" col in colSums

        uchar* resImgRow = resImg[i];
        for (size_t j = halfSize; j < endCol; ++j) {
            resImgRow[j] = (uchar)(allColsInKernelSum);
            allColsInKernelSum -= colSums[curPos];
            colSums[curPos] = columnSum(img, i, j, halfSize);
            allColsInKernelSum += colSums[curPos];
            ++curPos;
            curPos %= FILTER_SIZE;
        }
    }

    applyToBorder(img, resImg, height, width, halfSize, endRow);
}