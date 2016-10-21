/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains the constants to detect errors
 */

#include <stdint.h>
#include <stdbool.h>

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

// Type definitions
typedef struct _File File;
typedef struct _FilePool FilePool;

// Macros
#define SUCCESS                    0
#define FAIL                      -1
#define FILE_NOT_FOUND            -2
#define FILE_ALREADY_EXIST        -3
#define FILE_IS_ALREADY_OPENED    -4
#define FILE_IS_ALREADY_CLOSED    -5

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif
