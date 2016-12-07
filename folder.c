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
#include "hard_drive.h"

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

int32_t updateFolderDate(Folder *pFolder, char *newModDate)
{
    int32_t ret = SUCCESS;

    if (pFolder != NULL)
    {
        if (validateUser(pFolder->owner))
        {
            if (newModDate != NULL
                && (strcmp(pFolder->date, newModDate) != 0)
                && (strlen(newModDate) == strlen(pFolder->date)))
            {
                memset(pFolder->date, 0, sizeof(char)*MAX_DATE_SIZE);
                strcpy(pFolder->date, newModDate);
            }
            else
            {
                //update from the system
                getTimeStamp(pFolder->date); 
            }

            if (sendInfoToHD())
            {
                ret = updateFolderIntoHardDrive(pFolder);
            }
        }
        else
        {
            ret = INVALID_USER;
        }
    }

    return ret;
}

int32_t updateFolderOwner(Folder *pFolder, char *newOwner)
{
    int32_t ret = SUCCESS;

    if (pFolder != NULL
        && newOwner != NULL
        && (strcmp(pFolder->owner, newOwner) != 0))
    {
        if (isCurrentUserRoot())
        {
            memset(pFolder->owner, 0, sizeof(char) * MAX_OWNER_SIZE); 
            strcpy(pFolder->owner, newOwner);

            if (sendInfoToHD())
            {
                ret = updateFolderIntoHardDrive(pFolder);
            }
        }
        else
        {
            ret = INVALID_USER;
        }
    }

    return ret;
}

int32_t updateFolderPermissions(Folder *pFolder, uint16_t newPermissions)
{
    int32_t ret = SUCCESS;

    if (pFolder != NULL
        && pFolder->permissions != newPermissions)
    {
        if (validateUser(pFolder->owner))
        {
            pFolder->permissions = newPermissions; 

            if (sendInfoToHD())
            {
                ret = updateFolderIntoHardDrive(pFolder);
            }
        }
        else
        {
            ret = INVALID_USER;
        }
    }

    return ret; 
}

static int32_t freeFolderMemory(Folder *pFolder)
{
    int32_t ret = SUCCESS;

    if (pFolder != NULL)
    {
        if (validateUser(pFolder->owner))
        {
            if (sendInfoToHD()) 
            {
                ret = removeFolderIntoHardDrive(pFolder);
            }

            if (ret == SUCCESS)
            {
                ret = removeFileOrFolderFromPool(NULL, pFolder, true); 

                if (ret == SUCCESS)
                {
                    freeFolder(pFolder);
                }
            }
        }
        else
        {
            ret = INVALID_USER;
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

Folder * createNewFolder(Folder * parent, 
                         const char *pName, 
                         const char *owner, 
                         uint16_t permissions, 
                         const char *date,
                         DiskInfo *pDiskInfo,
                         int32_t  *retVal)
{
    Folder * newFolder = NULL;
    int32_t  ret = SUCCESS;

    if (pName != NULL)
    {
        ret = searchFileOrFolderIntoPool(parent, pName, NULL, NULL, true);

        if (ret == FOLDER_NOT_FOUND)
        {
            ret = searchFileOrFolderIntoPool(parent, pName, NULL, NULL, false);

            if (ret == FILE_NOT_FOUND)
            {
                newFolder = allocFolder(); 

                if (newFolder != NULL)
                {
                    newFolder->permissions = permissions;

                    strcpy(newFolder->name, pName);

                    if (owner != NULL)
                    {
                        strcpy(newFolder->owner, owner); 
                    }
                    else
                    {
                        strcpy(newFolder->owner, getCurrentUser());
                    }

                    if (date != NULL)
                    {
                        strcpy(newFolder->date, date);
                    }
                    else
                    {
                        getTimeStamp(newFolder->date); 
                    }

                    if (pDiskInfo != NULL)
                    {
                        memcpy(&(newFolder->diskInfo), pDiskInfo, sizeof(DiskInfo));
                    }
                    else
                    {
                        newFolder->diskInfo.cluster = NULL_CLUSTER; 
                        newFolder->diskInfo.dataSector = NULL_SECTOR;
                        newFolder->diskInfo.dataSize = 0;
                        newFolder->diskInfo.parentCluster = NULL_CLUSTER;
                        newFolder->diskInfo.childCluster = NULL_CLUSTER;
                        newFolder->diskInfo.nextCluster = NULL_CLUSTER;
                        newFolder->diskInfo.prevCluster = NULL_CLUSTER;
                    }

                    if (sendInfoToHD())
                    {
                        ret = createFolderIntoHardDrive(parent, newFolder);
                    }

                    ret = createNewFpool(newFolder, NULL, true, parent); 
                }
            }
            else
            {
                ret = FILE_ALREADY_EXIST;
            }
        }
        else
        {
            ret = FOLDER_ALREADY_EXIST;
        }
    }

    if (retVal != NULL)
    {
        *retVal = ret;
    }

    return newFolder;
}

int32_t destroyFolder(Folder * pFolder, bool recursive)
{
    int32_t ret = SUCCESS;

    if (pFolder != NULL)
    {
        if (validateUser(pFolder->owner))
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
        else
        {
            ret = INVALID_USER;
        }
    }

    return ret;
}

uint32_t getNumberOfChilds(Folder *pFolder)
{
    uint32_t ret = 0;

    if (pFolder != NULL
        && pFolder->fpool != NULL)
    {
        ret = pFolder->fpool->nElements;
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
            uint32_t index = 0;

            memset(permissions, 0, sizeof(char)*PERM_BUF_SIZE);

            if (pFolder->permissions & (WRITE_ONLY << 4))
            {
                permissions[index++] = 'w';
            }
            if (pFolder->permissions & (READ_ONLY << 4))
            {
                permissions[index++] = 'r';
            }
            if (pFolder->permissions & (EXEC_ONLY << 4))
            {
                permissions[index++] = 'x';
            }

            permissions[index++] = '-';

            if (pFolder->permissions & WRITE_ONLY)
            {
                permissions[index++] = 'w';
            }
            if (pFolder->permissions & READ_ONLY)
            {
                permissions[index++] = 'r';
            }
            if (pFolder->permissions & EXEC_ONLY)
            {
                permissions[index++] = 'x';
            }

            printf("%s\t %s\t %d\t %s\t %s\t %s\n",
                   permissions,
                   pFolder->owner,
                   0,
                   pFolder->date,
                   FOLDER_TAG,
                   pFolder->name); 
        }
        else
        {
            printf("%s\n", pFolder->name); 
        }
    }
}


