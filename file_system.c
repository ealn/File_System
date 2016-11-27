/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Implementation of methods to handle the file system
 */
#include <string.h>

#include "file_system.h"
#include "folder.h"
#include "interface.h"

#define MAX_USER_NAME         10
#define ROOT_USER             "root"
#define DEFAULT_USER          "user"
#define ROOT_FOLDER           "/"

Folder *g_rootFolder = NULL;
Folder *g_currentFolder = NULL;
char    currentUser[MAX_USER_NAME];
bool    sendInfoToHardDrive = false;

int32_t initFileSystem(void)
{
    int32_t ret = SUCCESS;

    ret = initFromHardDrive();

    if (ret == SUCCESS)
    {
        memset(currentUser, 0, sizeof(char) * MAX_USER_NAME); 
        strcpy(currentUser, DEFAULT_USER);
        sendInfoToHardDrive = true;
    }

    //TODO: remove this part when initFromHardDrive() is working
    if (1)
    {
        createRootFolder();
    }

    return ret;
}

char * getCurrentUser(void)
{
    return currentUser;
}

Folder * getCurrentFolder(void)
{
    return g_currentFolder;
}

bool sendInfoToHD(void)
{
    return sendInfoToHardDrive;
}

char * getCurrentFolderName(void)
{
    char * folderName = NULL;

    if (g_currentFolder != NULL)
    {
        folderName = g_currentFolder->name;
    }

    return folderName;
}

Folder * createRootFolder(void)
{
    Folder *pRootFolder = NULL;

    pRootFolder = createNewFolder(NULL, ROOT_FOLDER, NULL);
    g_rootFolder = pRootFolder;
    g_currentFolder = g_rootFolder;

    return pRootFolder;
}

void closeFileSystem(void)
{
    if (g_rootFolder != NULL)
    {
        //free the memory of whole file system
        destroyFolder(g_rootFolder, true);
    }
}


