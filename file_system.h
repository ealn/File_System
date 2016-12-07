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
bool setSetInfoToHD(bool value);
Folder * createRootFolder(char *date, DiskInfo *pDiskInfo);
Folder * getFolderFromPath(const char *path);
int32_t getLastNameFromPath(const char *path, char **output);
void setCurrentDirectory(Folder *pFolder);
char * getFullPath(Folder *pFolder);
Folder * getParentFolderOfFile(File *pFile);
Folder * getParentFolderOfFolder(Folder *pFolder);
Folder * getRootFolder(void);
int32_t getLastElementOfFolder(Folder *pFolder, File **pOutputFile, Folder **pOutputFolder);
int32_t restoreUser(void);
int32_t changeToRoot(const char *password);
bool isCurrentUserRoot(void);
bool validateUser(char *owner);
bool validatePermissions(uint16_t permissions);

//Defines
#define ROOT_FOLDER_NAME    "/"
#define FOLDER_SEPARATOR    '/'
#define PARENT_FOLDER       ".."
#define CURRENT_FOLDER      "."
#define ROOT_USER           "root"
#define DEFAULT_USER        "user"
#define MAX_USER_NAME       10
#define MAX_PASSWORD        10

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__FILE_SYSTEM_H__)
