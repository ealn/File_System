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
 * Porpuse: Implementation of methods to handle the file system
 */
#include <string.h>

#include "file_system.h"
#include "folder.h"
#include "f_pool.h"
#include "interface.h"
#include "console_utils.h"
#include "memutils.h"

#define DEFAULT_ROOT_PASSWORD "admin"

Folder *g_rootFolder = NULL;
Folder *g_currentFolder = NULL;

char          g_currentUser[MAX_USER_NAME];
char          g_rootPassword[MAX_PASSWORD];
bool          g_sendInfoToHardDrive = false;
bool          g_changeToRoot = false;

int32_t initFileSystem(void)
{
    int32_t ret = SUCCESS;

    memset(g_currentUser, 0, sizeof(char) * MAX_USER_NAME); 
    strcpy(g_currentUser, DEFAULT_USER);

    memset(g_rootPassword, 0, sizeof(char) * MAX_PASSWORD); 
    strcpy(g_rootPassword, DEFAULT_ROOT_PASSWORD);

    ret = initFromHardDrive();

    return ret;
}

char * getCurrentUser(void)
{
    return g_currentUser;
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
    g_sendInfoToHardDrive = value;
}

bool sendInfoToHD(void)
{
    return g_sendInfoToHardDrive;
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

Folder * createRootFolder(char *date, DiskInfo *pDiskInfo)
{
    Folder *pRootFolder = NULL;

    pRootFolder = createNewFolder(NULL, 
                                  ROOT_FOLDER_NAME, 
                                  ROOT_USER, 
                                  DEFAULT_PERMISSIONS, 
                                  date, 
                                  pDiskInfo, 
                                  NULL);
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

int32_t changeToRoot(const char *password)
{
    int32_t ret = SUCCESS;

    if (password != NULL
        && (strcmp(password, g_rootPassword) == 0))
    {
        memset(g_currentUser, 0, sizeof(char) * MAX_USER_NAME); 
        strcpy(g_currentUser, ROOT_USER);
        g_changeToRoot = true;
    }
    else
    {
        ret = INVALID_PASSWORD;
    }

    return ret;
}

int32_t restoreUser(void)
{
    int32_t ret = SUCCESS;

    if (g_changeToRoot)
    {
        memset(g_currentUser, 0, sizeof(char) * MAX_USER_NAME); 
        strcpy(g_currentUser, DEFAULT_USER);
        g_changeToRoot = false;
    }
    else
    {
        ret = FAIL;
    }

    return ret; 
}

bool isCurrentUserRoot(void)
{
    int32_t ret = false;

    if (strcmp(getCurrentUser(), ROOT_USER) == 0)
    {
        ret = true;
    }

    return ret;
}

bool validateUser(char *owner)
{
    int32_t ret = false;

    if (strcmp(getCurrentUser(), ROOT_USER) == 0
        || strcmp(getCurrentUser(), owner) == 0)
    {
        ret = true;
    }

    return ret;
}

bool validatePermissions(uint16_t permissions)
{
    int32_t ret = false;

    if (isCurrentUserRoot())
    {
        if (permissions & (WRITE_ONLY << 4))
        {
            ret = true;
        }
    }
    else
    {
        if (permissions & WRITE_ONLY)
        {
            ret = true;
        }
    }

    return ret;
}
