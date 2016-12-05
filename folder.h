/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains all functions to handle folders
 */

#include "defines.h"

#ifndef __FOLDER_H__
#define __FOLDER_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

Folder * createNewFolder(Folder * parent, 
                         const char *pName, 
                         const char *owner, 
                         uint16_t permissions, 
                         const char *date,
                         DiskInfo *pDiskInfo);
int32_t destroyFolder(Folder * pFolder, bool recursive);
void printFolderInfo(Folder * pFolder, bool showDetails);
uint32_t getNumberOfChilds(Folder *pFolder);
int32_t updateFolderDate(Folder *pFolder, char *newModDate);
int32_t updateFolderOwner(Folder *pFolder, char *newOwner);
int32_t updateFolderPermissions(Folder *pFolder, uint16_t newPermissions);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__FOLDER_H__)
