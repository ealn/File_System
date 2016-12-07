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
 * Porpuse: Implementation of interface between the file system and the hard drive 
 */

#include <stdio.h>
#include <string.h>

#include "interface.h"
#include "file_system.h"
#include "folder.h"
#include "hard_drive.h"

int32_t initFromHardDrive(void)
{
    int32_t ret = SUCCESS;

    ret = initHardDrive();

    return ret;
}

int32_t termHardDrive(void)
{
    int32_t ret = SUCCESS;

    ret = closeHardDrive();

    return ret;
}

int32_t createFolderIntoHardDrive(Folder *parentFolder, Folder *pFolder)
{
    int32_t ret = SUCCESS;

    ret = insertFolderIntoHD(parentFolder, pFolder);

    return ret;
}

int32_t removeFolderIntoHardDrive(Folder *pFolder)
{
    int32_t ret = SUCCESS;

    ret = removeFolderIntoHD(pFolder);

    return ret;
}

int32_t createFileIntoHardDrive(Folder *parentFolder, File *pFile)
{
    int32_t ret = SUCCESS;

    ret =  insertFileIntoHD(parentFolder, pFile);

    return ret;
}

int32_t removeFileIntoHardDrive(File *pFile)
{
    int32_t ret = SUCCESS;

    ret = removeFileIntoHD(pFile);

    return ret;
}

char *  readDataFromFile(File *pFile)
{
    char *ret = NULL;

    ret = readFileFromHD(pFile);

    return ret;
}

int32_t writeDataIntoFile(File *pFile, const char *newInfo)
{
    int32_t ret = SUCCESS;

    ret = writeFileIntoHD(pFile, newInfo);

    return ret;
}

int32_t updateFileIntoHardDrive(File *pFile)
{
    int32_t ret = SUCCESS;

    ret = modifyFileIntoHD(pFile);

    return ret;
}

int32_t updateFolderIntoHardDrive(Folder *pFolder)
{
    int32_t ret = SUCCESS;

    ret = modifyFolderIntoHD(pFolder);

    return ret;
}

