/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains all functions to the file pool
 */

#include "defines.h"

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

int32_t createFilePool(void);
void destroyFilePool(void);
int32_t insertFileIntoPool(File * pFile);
int32_t removeFileIntoPool(File * pFile);
int32_t searchFileIntoPool(char *pName, File **ppOutputFile);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif
