/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains all functions to the files
 */
#include "defines.h"

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

#define FILE_NAME_MAX_SIZE    32

int32_t createNewFile(char *pName);
int32_t openFile(char *pName);
int32_t closeFile(char *pName);
int32_t deleteFile(char *pName);
int32_t writeFile(char *pName, char *input, uint32_t numberOfBytes);
int32_t readFile(char *pName, char *output, uint32_t numberOfBytes);
int32_t tellFile(char *pName, uint32_t *output);
int32_t seekFile(char *pName, uint32_t newPoint);
void destroyFile(File * pFile);
char * getFileName(File *pFile);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif
