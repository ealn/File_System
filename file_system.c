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
#include "memutils.h"

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

Folder * getRootFolder(void)
{
    return g_rootFolder;
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
        sendInfoToHardDrive = false;

        //free the memory of whole file system
        destroyFolder(g_rootFolder, true);
        getchar();
    }
}

Folder * getFolderFromPath(const char *path)
{
    Folder * pFolder = NULL;
    char   **listOfParms = NULL;
    uint32_t numberOfParms = 0;
    uint32_t i = 0;
    int32_t  ret = 0;
    char    *copyPath;
    uint32_t len = 0;

    if (path != NULL)
    {
        len = strlen(path) + 1; //Add null terminator

        copyPath = (char *)MEMALLOC(sizeof(char)*len);

        if (copyPath != NULL)
        {
            //if this is absolute path
            if (path[0] == FOLDER_SEPARATOR)
            {
                pFolder = g_rootFolder;
                strcpy(copyPath, &path[1]);
            }
            else  //else this is a relative path
            {
                pFolder = g_currentFolder;
                strcpy(copyPath, path);
            }

            parseString(copyPath, FOLDER_SEPARATOR, &listOfParms, &numberOfParms);

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

                    if (ret == FOLDER_NOT_FOUND)
                    {
                        pFolder = NULL;
                    }
                }

                destroyStringsParsed(listOfParms, numberOfParms);
            }

            MEMFREE((void *)copyPath);
        }
    }

    return pFolder;
}

int32_t getLastNameFromPath(const char *path, char **output)
{
    int32_t ret = SUCCESS;

    if (path != NULL
        && output != NULL)
    {
        char    **listOut = NULL;
        uint32_t  numberOfElements = 0;
        uint32_t  length = 0;
        char     *copyPath;
        uint32_t  len = 0;

        len = strlen(path) + 1; //Add null terminator

        copyPath = (char *)MEMALLOC(sizeof(char)*len);

        if (copyPath != NULL)
        {
            //if this is absolute path
            if (path[0] == FOLDER_SEPARATOR)
            {
                strcpy(copyPath, &path[1]);
            }
            else  //else this is a relative path
            {
                strcpy(copyPath, path);
            }

            parseString(copyPath, FOLDER_SEPARATOR, &listOut, &numberOfElements);

            if (listOut != NULL)
            {
                length = strlen(listOut[numberOfElements - 1]) + 1;

                *output = (char *)MEMALLOC(sizeof(char) * length);
                memcpy(*output, listOut[numberOfElements - 1], sizeof(char) * (length - 1));
                destroyStringsParsed(listOut, numberOfElements);
            }

            MEMFREE((void *)copyPath);
        }
    }

    return ret; 
}

void setCurrentDirectory(Folder *pFolder)
{
    g_currentFolder = pFolder;
}

char * getFullPath(Folder *pFolder)
{
    char * path = NULL;

    return path;
}

Folder * getParentFolderOfFile(File *pFile)
{
    Folder *parent = NULL;
    Fpool  *parentPool = NULL;

    if (pFile != NULL)
    {
        parentPool = pFile->fpool->parent;

        if (parentPool != NULL)
        {
            parent = parentPool->folder;
        }
    }

    return parent;
}

Folder * getParentFolderOfFolder(Folder *pFolder)
{
    Folder *parent = NULL;
    Fpool  *parentPool = NULL;

    if (pFolder != NULL)
    {
        parentPool = pFolder->fpool->parent;

        if (parentPool != NULL)
        {
            parent = parentPool->folder;
        }
    }

    return parent;
}
