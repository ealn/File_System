/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains all functions to handle files
 */

#include "defines.h"
#include <stdio.h>

#ifndef __FILE_H__
#define __FILE_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

File * createNewFile(Folder *parent, char *pName, uint16_t permissions);
int32_t destroyFile(File * pFile);
void printFileInfo(File *pFile, bool showDetails);
int32_t updateFileModificationDate(File *pFile, char *newModDate);
int32_t copyFiles(File *srcFile, Folder *dstFolder);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__FILE_H__)


