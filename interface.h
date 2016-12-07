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
 * Porpuse: Header that contains the interface between the file system and the hard drive
 */

#include "defines.h"

#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

int32_t initFromHardDrive(void);
int32_t termHardDrive(void);
int32_t createFolderIntoHardDrive(Folder *parentFolder, Folder *pFolder);
int32_t removeFolderIntoHardDrive(Folder *pFolder);
int32_t createFileIntoHardDrive(Folder *parentFolder, File *pFile);
int32_t removeFileIntoHardDrive(File *pFile);
char *  readDataFromFile(File *pFile);
int32_t writeDataIntoFile(File *pFile, const char *newInfo);
int32_t updateFileIntoHardDrive(File *pFile);
int32_t updateFolderIntoHardDrive(Folder *pFolder);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__INTERFACE_H__)
