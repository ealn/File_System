/*
 * Copyright (c) 2016 by Emmanuel
 * All Rights Reserved
 *
 * Author: Emmanuel
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

int32_t createFolderIntoHardDrive(Folder *pFolder)
{
    int32_t ret = SUCCESS;

    //TODO

    return ret;
}

int32_t removeFolderIntoHardDrive(Folder *pFolder)
{
    int32_t ret = SUCCESS;

    //TODO

    return ret;
}

int32_t createFileIntoHardDrive(File *pFile)
{
    int32_t ret = SUCCESS;

    //TODO

    return ret;
}

int32_t removeFileIntoHardDrive(File *pFile)
{
    int32_t ret = SUCCESS;

    //TODO

    return ret;
}

char *  readDataFromFile(File *pFile)
{
    char *ret = NULL;

    //TODO

    return ret;
}

int32_t writeDataIntoFile(File *pFile, char *newInfo)
{
    int32_t ret = SUCCESS;

    //TODO

    return ret;
}

//This function will be used to update information
//of file into hard drive for example when the shell
//modifies the date.
int32_t updateFileIntoHardDrive(File *pFile)
{
    int32_t ret = SUCCESS;

    return ret;
}

