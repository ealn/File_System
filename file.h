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
 * Porpuse: Header that contains all functions to handle files
 */

#include "defines.h"
#include <stdio.h>

#ifndef __FILE_H__
#define __FILE_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

File * createNewFile(Folder *parent, 
                     const char *pName, 
                     const char *owner, 
                     uint16_t permissions, 
                     const char *date,
                     DiskInfo *pDiskInfo,
                     int32_t *retVal);
int32_t destroyFile(File * pFile);
void printFileInfo(File *pFile, bool showDetails);
int32_t updateFileDate(File *pFile, char *newModDate);
int32_t updateFileOwner(File *pFile, char *newOwner);
int32_t updateFilePermissions(File *pFile, uint16_t newPermissions);
int32_t copyFiles(File *srcFile, Folder *dstFolder);
char * readFile(Folder *parent, char *pName);
int32_t writeFile(Folder *parent, char *pName, const char *input);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__FILE_H__)


