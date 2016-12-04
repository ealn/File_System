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

Folder *g_rootFolder = NULL;
Folder *g_currentFolder = NULL;
char    currentUser[MAX_USER_NAME];
bool    sendInfoToHardDrive = false;

int32_t initFileSystem(void)
{
    int32_t ret = SUCCESS;

    memset(currentUser, 0, sizeof(char) * MAX_USER_NAME); 
    strcpy(currentUser, DEFAULT_USER);

    ret = initFromHardDrive();


    //TODO: remove this part when initFromHardDrive() is working
    /*if (1)
    {
        createRootFolder();
    }*/

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

bool setSetInfoToHD(bool value)
{
    sendInfoToHardDrive = value;
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
        setSetInfoToHD(false);

        //free the memory of whole file system
        destroyFolder(g_rootFolder, true);
        termHardDrive();
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
    char   * path = NULL;
    Folder * tempFolder = NULL;
    Folder **folderArray = NULL;
    uint32_t n = 0;
    int32_t  i = 0;
    uint32_t index = 0;
    uint32_t len = 0;

    if (pFolder != NULL)
    {
        tempFolder = pFolder;

        while (tempFolder != NULL)
        {
            len += strlen(tempFolder->name);
            n++;
            tempFolder = getParentFolderOfFolder(tempFolder); 
        }

        len += n  + 1; //add the directory separator and the null terminator
        path = (char *)MEMALLOC(sizeof(char)*len);

        if (path != NULL)
        {
            folderArray = (Folder **)MEMALLOC(sizeof(Folder *)*n);

            if (folderArray != NULL)
            {
                tempFolder = pFolder; 

                //insert elements to array
                while (tempFolder != NULL)
                {
                    folderArray[i] = tempFolder;
                    tempFolder = getParentFolderOfFolder(tempFolder);

                    //if there is more folders
                    if (tempFolder != NULL)
                    {
                        i++;
                    }
                }

                while (i >= 0)
                {
                    len = strlen(folderArray[i]->name);
                    strcpy(&path[index], folderArray[i]->name);

                    index += len;

                    //if the folder is not root and the index is not the last element
                    if (!(strcmp(folderArray[i]->name, ROOT_FOLDER_NAME) == 0)
                        && (i > 0))
                    {
                        //add the directory separator
                        path[index] = FOLDER_SEPARATOR;
                        index++;
                    }

                    i--; 
                }

                MEMFREE((void *)folderArray);
            }
        }
    }

    return path;
}

Folder * getParentFolderOfFile(File *pFile)
{
    Folder *parent = NULL;
    Fpool  *parentPool = NULL;

    if (pFile != NULL
        && pFile->fpool != NULL)
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

    if (pFolder != NULL
        && pFolder->fpool != NULL)
    {
        parentPool = pFolder->fpool->parent;

        if (parentPool != NULL)
        {
            parent = parentPool->folder;
        }
    }

    return parent;
}

int32_t getLastElementOfFolder(Folder *pFolder, File **pOutputFile, Folder **pOutputFolder)
{
    if (pFolder != NULL)
    {
        if (pOutputFile != NULL)
        {
            *pOutputFile = NULL;
        }
        if (pOutputFolder != NULL)
        {
            *pOutputFolder = NULL;
        }

        if (pFolder->fpool != NULL
            && pFolder->fpool->last != NULL) 
        {
            if (pFolder->fpool->last->isDir
                && pOutputFolder != NULL)
            {
                *pOutputFolder = pFolder->fpool->last->folder;
            }
            else if (!pFolder->fpool->last->isDir
                     && pOutputFile != NULL)
            {
                *pOutputFile = pFolder->fpool->last->file;
            }
        }
    }
}
