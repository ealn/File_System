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

int32_t initFromHardDrive(void)
{
    int32_t ret = SUCCESS;
    Folder *root = NULL;
    Folder *pFolder = NULL;
    File   *pFile = NULL;

    //TODO
    /* root = createRootFolder();
       while ()
       {
          TODO
          if ()
          {
             pFolder = createNewFolder(...);
          }
          else
          {
             pFile = createNewFile(...);
          }
       }
    */
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
