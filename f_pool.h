/*
 * Copyright (c) 2016 by Efrain Adrian Luna Nevarez
 *                       Emmanuel Salcido Maldonado
 *                       Jesus Eduardo Silva Padilla
 *                       Efrain Arrambide Barron
 *                       Ricardo Isaac Gonzalez Ordaz
 *                       Victor Antonio Morales Carrillo
 * All Rights Reserved
 *
 * Authors: Efrain Adrian Luna Nevarez
 *          Emmanuel Salcido Maldonado
 *          Jesus Eduardo Silva Padilla
 *          Efrain Arrambide Barron
 *          Ricardo Isaac Gonzalez Ordaz
 *          Victor Antonio Morales Carrillo
 *
 * Porpuse: Header that contains all functions to the file pool
 */

#include "defines.h"

#ifndef __F_POOL_H__
#define __F_POOL_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

int32_t createNewFpool(Folder *pFolder, File *pFile, bool isDir, Folder *pParentFolder);
int32_t searchFileOrFolderIntoPool(Folder     *parentFolder,
                                   const char *pName,
                                   File      **ppOutputFile,
                                   Folder    **ppOutputFolder,
                                   bool        searchDir);
int32_t removeFileOrFolderFromPool(File * pFile, Folder *pFolder, bool isDir);
int32_t printInfoOfPool(Folder * pFolder, bool showDetails);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__F_POOL_H__)
