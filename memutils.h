/*
 * Copyright (c) 2016 by Efrain Adrian Luna Nevarez
 *                       Emmanuel Salcido Maldonado
 *                       Jesus Eduardo Silva Padilla
 *                       Efrain Arrambide Barron
 *                       Ricardo Isaac Gonzalez Ordaz
 * All Rights Reserved
 *
 * Authors: Efrain Adrian Luna Nevarez
 *          Emmanuel Salcido Maldonado
 *          Jesus Eduardo Silva Padilla
 *          Efrain Arrambide Barron
 *          Ricardo Isaac Gonzalez Ordaz
 *
 * Porpuse: Header that contains all functions to allocate and free memory
 */

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#ifndef __MEM_UTILS_H__
#define __MEM_UTILS_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

// Functions:
void * memAlloc(size_t size,
                const char * func,
                const char * file,
                int32_t line);

void memFree(void * ptr,
             const char * func,
             const char * file,
             int32_t line);

void * memRealloc(void *ptr, 
                  size_t origSize, 
                  size_t newSize,
                  const char * func,
                  const char * file,
                  int32_t line);

// Macros:
#define MEMALLOC(size)                       memAlloc((size), __func__, __FILE__, __LINE__)
#define MEMFREE(ptr)                         memFree((ptr), __func__, __FILE__, __LINE__)
#define MEMREALLOC(ptr, origSize, newSize)   memRealloc((ptr), (origSize), (newSize), __func__, __FILE__, __LINE__)

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__MEM_UTILS_H__)
