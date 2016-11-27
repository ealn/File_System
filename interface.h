/*
 * Copyright (c) 2016 by Emmanuel
 * All Rights Reserved
 *
 * Author: Emmanuel
 *
 * Porpuse: Header that contains the interface between the file system and the hard drive
 */

#include "defines.h"

#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

int32_t initFromHardDrive(void);
int32_t createFolderIntoHardDrive(Folder *pFolder);
int32_t removeFolderIntoHardDrive(Folder *pFolder);
int32_t createFileIntoHardDrive(File *pFile);
int32_t removeFileIntoHardDrive(File *pFile);
char *  readDataFromFile(File *pFile);
int32_t writeDataIntoFile(File *pFile, char *newInfo);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__INTERFACE_H__)
