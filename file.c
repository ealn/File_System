/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Implementation of methods to handle files
 */

#include <string.h>

#include "file.h"
#include "f_pool.h"
#include "file_system.h"
#include "memutils.h"
#include "interface.h"
#include "hard_drive.h"

static File * allocFile(void)
{
    File * pFile = NULL;
    
    pFile = (File *)MEMALLOC(sizeof(File));
    
    return pFile;
}

static void freeFile(File *pFile)
{
    if (pFile != NULL)
    {
        MEMFREE((void *)pFile);
    }
}

void printFileInfo(File *pFile, bool showDetails)
{
    if (pFile != NULL)
    {
        if (showDetails)
        {
            char permissions[PERM_BUF_SIZE];
            uint32_t index = 0;

            memset(permissions, 0, sizeof(char)*PERM_BUF_SIZE);

            if (pFile->permissions & (WRITE_ONLY << 4))
            {
                permissions[index++] = 'w';
            }
            if (pFile->permissions & (READ_ONLY << 4))
            {
                permissions[index++] = 'r';
            }
            if (pFile->permissions & (EXEC_ONLY << 4))
            {
                permissions[index++] = 'x';
            }

            permissions[index++] = '-';

            if (pFile->permissions & WRITE_ONLY)
            {
                permissions[index++] = 'w';
            }
            if (pFile->permissions & READ_ONLY)
            {
                permissions[index++] = 'r';
            }
            if (pFile->permissions & EXEC_ONLY)
            {
                permissions[index++] = 'x';
            }

            printf("%s\t %s\t %d\t %s\t %s\t %s\n",
                   permissions,
                   pFile->owner,
                   pFile->diskInfo.dataSize,
                   pFile->date,
                   "",
                   pFile->name); 
        }
        else
        {
            printf("%s\n", pFile->name); 
        }
    }
}

int32_t openFile(Folder *parent, char *pName)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (parent != NULL
        && pName != NULL)
    {
        ret = searchFileOrFolderIntoPool(parent, pName, &pFile, NULL, false);

        if (pFile != NULL)
        {
            /* TODO
            if (!pFile->is_opened)
            {
                pFile->is_opened = true; 
            }
            else
            {
                ret = FILE_IS_ALREADY_OPENED;
            }*/
        }
    }
    else
    {
        ret = FAIL;
    }

    return ret; 
}

int32_t closeFile(Folder *parent, char *pName)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (parent != NULL
        && pName != NULL)
    {
        ret = searchFileOrFolderIntoPool(parent, pName, &pFile, NULL, false);

        if (pFile != NULL)
        {
            /* TODO:
            if (pFile->is_opened)
            {
                pFile->is_opened = false; 
            }
            else
            {
                ret = FILE_IS_ALREADY_CLOSED;
            }*/
        }
    }
    else
    {
        ret = FAIL;
    }

    return ret; 
}

int32_t writeFile(Folder *parent, char *pName, char *input)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (parent != NULL
        && pName != NULL
        && input != NULL)
    {
        uint32_t index = 0;

        ret = searchFileOrFolderIntoPool(parent, pName, &pFile, NULL, false);

        if (ret == SUCCESS
            && pFile != NULL)
        {
            ret = writeDataIntoFile(pFile, input);
        }
    }
    else
    {
        ret = FAIL;
    }

    return ret; 
}

char * readFile(Folder *parent, char *pName)
{
    char * output = NULL;
    File   *pFile = NULL;
    int32_t ret = SUCCESS;

    if (parent != NULL
        && pName != NULL)
    {
        ret = searchFileOrFolderIntoPool(parent, pName, &pFile, NULL, false);

        if (ret == SUCCESS
            && pFile != NULL)
        {
            output = readDataFromFile(pFile);
        }
    }

    return output; 
}

int32_t tellFile(Folder *parent, char *pName, uint32_t *output)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (parent != NULL
        && pName != NULL
        && output != NULL)
    {
        ret = searchFileOrFolderIntoPool(parent, pName, &pFile, NULL, false);

        if (pFile != NULL)
        {
            /*TODO
            *output = pFile->r_point;*/
        }
    }
    else
    {
        ret = FAIL;
    }

    return ret; 
}

int32_t seekFile(Folder *parent, char *pName, uint32_t newPoint)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (parent != NULL
        && pName != NULL)
    {
        ret = searchFileOrFolderIntoPool(parent, pName, &pFile, NULL, false);

        if (pFile != NULL)
        {
            /*TODO
            if (newPoint < pFile->size)
            {
                pFile->r_point = newPoint;
            }
            else
            {
                ret = FAIL;
            }*/
        }
    }
    else
    {
        ret = FAIL;
    }

    return ret; 
}

int32_t updateFileDate(File *pFile, char *newModDate)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL)
    {
        if (newModDate != NULL
            && (strcmp(pFile->date, newModDate) != 0)
            && (strlen(newModDate) == strlen(pFile->date)))
        {
            memset(pFile->date, 0, sizeof(char)*MAX_DATE_SIZE);
            strcpy(pFile->date, newModDate);
        }
        else
        {
            //update from the system
            getTimeStamp(pFile->date); 
        }

        if (sendInfoToHD())
        {
            ret = updateFileIntoHardDrive(pFile);
        }
    }

    return ret;
}

int32_t updateFileOwner(File *pFile, char *newOwner)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL
        && newOwner != NULL
        && (strcmp(pFile->owner, newOwner) != 0))
    {
        memset(pFile->owner, 0, sizeof(char)*MAX_OWNER_SIZE);
        strcpy(pFile->owner, newOwner);

        if (sendInfoToHD())
        {
            ret = updateFileIntoHardDrive(pFile);
        }
    }

    return ret;
}

int32_t updateFilePermissions(File *pFile, uint16_t newPermissions)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL
        && pFile->permissions != newPermissions)
    {
        pFile->permissions = newPermissions;

        if (sendInfoToHD())
        {
            ret = updateFileIntoHardDrive(pFile);
        }
    }

    return ret; 
}

int32_t copyFiles(File *srcFile, Folder *dstFolder)
{
    int32_t ret = SUCCESS;

    if (srcFile != NULL
        && dstFolder != NULL)
    {
        File *newFile = NULL;
        char * data = NULL;

        newFile = createNewFile(dstFolder, 
                                srcFile->name,
                                srcFile->owner,
                                srcFile->permissions,
                                srcFile->date,
                                NULL);

        data = readDataFromFile(srcFile);

        if (data != NULL)
        {
            ret = writeDataIntoFile(newFile, data);
        }
    }

    return ret; 
}

File * createNewFile(Folder *parent, 
                     const char *pName, 
                     const char *owner, 
                     uint16_t permissions, 
                     const char *date,
                     DiskInfo *pDiskInfo)
{
    File   * newFile = NULL;
    int32_t  ret = SUCCESS;

    if (pName != NULL)
    {
        ret = searchFileOrFolderIntoPool(parent, pName, NULL, NULL, false);

        if (ret == FILE_NOT_FOUND)
        {
            ret = searchFileOrFolderIntoPool(parent, pName, NULL, NULL, true);

            if (ret == FOLDER_NOT_FOUND)
            {
                newFile = allocFile(); 

                if (newFile != NULL)
                {
                    newFile->permissions = permissions;

                    strcpy(newFile->name, pName);

                    if (owner != NULL)
                    {
                        strcpy(newFile->owner, owner); 
                    }
                    else
                    {
                        strcpy(newFile->owner, getCurrentUser());
                    }

                    if (date != NULL)
                    {
                        strcpy(newFile->date, date);
                    }
                    else
                    {
                        getTimeStamp(newFile->date); 
                    }

                    if (pDiskInfo != NULL)
                    {
                        memcpy(&(newFile->diskInfo), pDiskInfo, sizeof(DiskInfo));
                    }
                    else
                    {
                        newFile->diskInfo.cluster = NULL_CLUSTER; 
                        newFile->diskInfo.dataSector = NULL_SECTOR;
                        newFile->diskInfo.dataSize = 0;
                        newFile->diskInfo.parentCluster = NULL_CLUSTER;
                        newFile->diskInfo.childCluster = NULL_CLUSTER;
                        newFile->diskInfo.nextCluster = NULL_CLUSTER;
                        newFile->diskInfo.prevCluster = NULL_CLUSTER;
                    }

                    if (sendInfoToHD())
                    {
                        ret = createFileIntoHardDrive(parent, newFile, NULL);
                    }

                    ret = createNewFpool(NULL, newFile, false, parent); 
                }
            }
            else
            {
                ret = FOLDER_ALREADY_EXIST;
            }
        }
        else
        {
            ret = FILE_ALREADY_EXIST;
        }
    }
    
    return newFile;
}

int32_t destroyFile(File * pFile)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL)
    {
        if (sendInfoToHD())
        {
            ret = removeFileIntoHardDrive(pFile);
        }

        if (ret == SUCCESS)
        {
            ret = removeFileOrFolderFromPool(pFile, NULL, false); 

            if (ret == SUCCESS)
            {
                freeFile(pFile);
            }
        }
    }
    else
    {
        ret = FAIL;
    }

    return ret;
}
