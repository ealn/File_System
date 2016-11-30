/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Implementation of methods to handle folders
 */

#include <string.h>

#include "folder.h"
#include "f_pool.h"
#include "memutils.h"
#include "trace.h"
#include "file.h"
#include "file_system.h"
#include "interface.h"

//Defines
#define FOLDER_TAG    "<DIR>"

//Implementation
static Folder * allocFolder(void)
{
    Folder * pFolder = NULL;
    
    pFolder = (Folder *)MEMALLOC(sizeof(Folder));
    
    return pFolder;
}

static void freeFolder(Folder *pFolder)
{
    if (pFolder != NULL)
    {
        MEMFREE((void *)pFolder);
    }
}

static int32_t freeFolderMemory(Folder *pFolder)
{
    int32_t ret = SUCCESS;

    if (pFolder != NULL)
    {
        ret = removeFileOrFolderFromPool(NULL, pFolder, true);

        if (ret == SUCCESS
            && sendInfoToHD())
        {
            ret = removeFolderIntoHardDrive(pFolder);
        }

        if (ret == SUCCESS)
        {
            if (pFolder->name != NULL) 
            {
                MEMFREE((void *)pFolder->name); 
            }
            if (pFolder->owner != NULL)
            {
                MEMFREE((void *)pFolder->owner);
            }

            freeFolder(pFolder);
        }
    }
    else
    {
        ret = FAIL;
    }

    return ret;
}

int32_t destroyFolderRecursive(Fpool *pFpool)
{
    int32_t ret = SUCCESS;

    if (pFpool != NULL)
    {
        Fpool * next;
        Fpool * child;

        next = pFpool->next;
        child = pFpool->child;

        //go to the childs
        if (child != NULL)
        {
            ret = destroyFolderRecursive(child);
        }

        //destroy element
        if (pFpool->isDir)
        {
            ret = freeFolderMemory(pFpool->folder);
        }
        else
        {
            ret = destroyFile(pFpool->file);
        }

        //check if there is more elements to the right
        if (next != NULL)
        {
            // send the next element
            ret = destroyFolderRecursive(next);
        }
    }

    return ret;
}

Folder * createNewFolder(Folder * parent, const char *pName, const char *pCreationDate)
{
    Folder * newFolder = NULL;
    uint32_t nameSize = 0;
    int32_t  ret = SUCCESS;
    char    *currentUser = NULL;
    uint32_t curUserSize = 0;

    if (pName != NULL)
    {
        ret = searchFileOrFolderIntoPool(parent, pName, NULL, NULL, true);

        if (ret == FOLDER_NOT_FOUND)
        {
            newFolder = allocFolder(); 
            nameSize = strlen(pName) + 1;  //add the \0 character

            //TODO: send the user as parameter
            currentUser = getCurrentUser();
            curUserSize = strlen(currentUser) + 1;  //add the \0 character

            if (newFolder != NULL)
            {
                newFolder->name =  (char *)MEMALLOC(sizeof(char)* nameSize);
                newFolder->owner = (char *)MEMALLOC(sizeof(char)* curUserSize);

                strcpy(newFolder->name, pName);
                strcpy(newFolder->owner, currentUser);

                //TODO: send permissions as parameter
                newFolder->permissions = READ_ONLY | WRITE_ONLY | EXEC_ONLY;

                if (pCreationDate == NULL)
                {
                    getTimeStamp(newFolder->c_date); 
                }
                else
                {
                    memcpy(newFolder->c_date, pCreationDate, (TIME_BUF_SIZE - 1));
                }

                ret = createNewFpool(newFolder, NULL, true, parent);

                if (ret == SUCCESS
                    && sendInfoToHD())
                {
                    createFolderIntoHardDrive(newFolder);
                }
            }
        }
    }
    
    return newFolder;
}

int32_t destroyFolder(Folder * pFolder, bool recursive)
{
    int32_t ret = SUCCESS;

    if (pFolder != NULL)
    {
        if (recursive)
        {
            ret = destroyFolderRecursive(pFolder->fpool);
        }
        else 
        {
            ret = freeFolderMemory(pFolder); 
        }
    }

    return ret;
}

void printFolderInfo(Folder * pFolder, bool showDetails)
{
    if (pFolder != NULL)
    {
        if (showDetails)
        {
            char permissions[PERM_BUF_SIZE];

            memset(permissions, 0, sizeof(char)*PERM_BUF_SIZE);

            if (pFolder->permissions & WRITE_ONLY)
            {
                permissions[0] = 'w';
            }
            if (pFolder->permissions & READ_ONLY)
            {
                permissions[1] = 'r';
            }
            if (pFolder->permissions & EXEC_ONLY)
            {
                permissions[2] = 'x';
            }

            printf("%s\t %s\t %d\t %s\t %s\t %s\n",
                   permissions,
                   pFolder->owner,
                   0,
                   pFolder->c_date,
                   FOLDER_TAG,
                   pFolder->name); 
        }
        else
        {
            printf("%s\n", pFolder->name); 
        }
    }
}


