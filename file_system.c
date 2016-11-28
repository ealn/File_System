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
#include "f_pool.h"
#include "interface.h"
#include "console_utils.h"

#define MAX_USER_NAME         10
#define ROOT_USER             "root"
#define DEFAULT_USER          "user"

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

    pRootFolder = createNewFolder(NULL, ROOT_FOLDER_NAME, NULL);
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

Folder * getFolderFromPath(char *path)
{
    Folder * pFolder = NULL;
    char   **listOfParms = NULL;
    uint32_t numberOfParms = 0;
    uint32_t i = 0;
    int32_t  ret = 0;

    if (path != NULL)
    {
        parseString(path, FOLDER_SEPARATOR, &listOfParms, &numberOfParms);

        if (path[0] == FOLDER_SEPARATOR)
        {
            pFolder = g_rootFolder;
        }
        else
        {
            pFolder = g_currentFolder; 
        }

        if (listOfParms != NULL)
        {
            if (numberOfParms > 1)
            {
                for (i = 0; i < (numberOfParms - 1); i++)
                {
                    ret = searchFileOrFolderIntoPool(pFolder,
                                                     listOfParms[i],
                                                     NULL,
                                                     &pFolder,
                                                     true);
                }

                if (ret == FILE_NOT_FOUND)
                {
                    pFolder = NULL;
                }
            }

            destroyStringsParsed(listOfParms, numberOfParms);
        }

    }

    return pFolder;
}

void setCurrentDirectory(Folder *pFolder)
{
    g_currentFolder = pFolder;
}

