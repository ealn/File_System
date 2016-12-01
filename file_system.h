/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains all functions of file system
 */

#include "defines.h"
#include <stdio.h>

#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

int32_t initFileSystem(void);
void closeFileSystem(void);
char * getCurrentUser(void);
Folder * getCurrentFolder(void);
char * getCurrentFolderName(void);
bool sendInfoToHD(void);
Folder * createRootFolder(void);
Folder * getFolderFromPath(const char *path);
int32_t getLastNameFromPath(const char *path, char **output);
void setCurrentDirectory(Folder *pFolder);
char * getFullPath(Folder *pFolder);
Folder * getParentFolderOfFile(File *pFile);
Folder * getParentFolderOfFolder(Folder *pFolder);
Folder * getRootFolder(void);

//Defines
#define ROOT_FOLDER_NAME    "/"
#define FOLDER_SEPARATOR    '/'
#define PARENT_FOLDER       ".."
#define CURRENT_FOLDER      "."
#define ROOT_USER           "root"
#define DEFAULT_USER        "user"

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__FILE_SYSTEM_H__)
