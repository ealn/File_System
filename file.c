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

            memset(permissions, 0, sizeof(char)*PERM_BUF_SIZE);

            if (pFile->permissions & WRITE_ONLY)
            {
                permissions[0] = 'w';
            }
            if (pFile->permissions & READ_ONLY)
            {
                permissions[1] = 'r';
            }
            if (pFile->permissions & EXEC_ONLY)
            {
                permissions[2] = 'x';
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

int32_t writeFile(Folder *parent, char *pName, char *input, uint32_t numberOfBytes)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (parent != NULL
        && pName != NULL
        && input != NULL
        && numberOfBytes >= 1)
    {
        uint32_t index = 0;

        ret = searchFileOrFolderIntoPool(parent, pName, &pFile, NULL, false);

        if (pFile != NULL)
        {
            /* TODO:
            index = pFile->w_point;

            while (index + numberOfBytes > (pFile->size - 1))  //keep the \0 terminator
            {
                changeFileSize(pFile, pFile->size + CHUNK_SIZE);
            }
            
            memcpy(&pFile->data[index], input, numberOfBytes);

            pFile->w_point += numberOfBytes;

            getTimeStamp(pFile->date);

            TODO:
            if (ret == SUCCESS
                && sendInfoToHD())
            {
                ret = writeDataIntoFile(pFile, pFile->data);
            }*/
        }
    }
    else
    {
        ret = FAIL;
    }

    return ret; 
}

int32_t readFile(Folder *parent, char *pName, char *output, uint32_t numberOfBytes)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (parent != NULL
        && pName != NULL
        && output != NULL)
    {
        uint32_t index = 0;

        ret = searchFileOrFolderIntoPool(parent, pName, &pFile, NULL, false);

        if (pFile != NULL)
        {
            /* TODO
            index = pFile->r_point;

            if (index + numberOfBytes > pFile->size)
            {
                memcpy(output, &pFile->data[index], pFile->size);
            }
            else
            {
                memcpy(output, &pFile->data[index], numberOfBytes);
            }

            pFile->r_point += numberOfBytes;

            //TODO:
            //output = readDataFromFile(pFile);*/
        }
    }
    else
    {
        ret = FAIL;
    }

    return ret; 
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

int32_t updateFileModificationDate(File *pFile, char *newModDate)
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
        File *newFile;

        newFile = createNewFile(dstFolder, srcFile->name, srcFile->permissions);

        if (newFile != NULL)
        {
            //TODO: get Data from hard disk
            //updateFileData(newFile, srcFile->data, srcFile->size, srcFile->w_point);
            updateFileModificationDate(newFile, srcFile->date);
            updateFileOwner(newFile, srcFile->owner);
        }
        else
        {
            ret = FAIL;
        }
    }

    return ret; 
}

File * createNewFile(Folder *parent, char *pName, uint16_t permissions)
{
    File * newFile = NULL;
    uint32_t nameSize = 0;
    int32_t  ret = SUCCESS;
    char    *currentUser = NULL;
    uint32_t curUserSize = 0;

    if (pName != NULL)
    {
        ret = searchFileOrFolderIntoPool(parent, pName, NULL, NULL, false);

        if (ret == FILE_NOT_FOUND)
        {
            newFile = allocFile(); 
            nameSize = strlen(pName) + 1;  //add the \0 character

            //TODO: send the user as parameter
            currentUser = getCurrentUser();
            curUserSize = strlen(currentUser) + 1;  //add the \0 character

            if (newFile != NULL)
            {
                //TODO: send permissions as parameter
                newFile->permissions = DEFAULT_PERMISSIONS;

                strcpy(newFile->name, pName);
                strcpy(newFile->owner, currentUser);

                getTimeStamp(newFile->date);

                newFile->diskInfo.cluster = NULL_CLUSTER;
                newFile->diskInfo.dataSector = NULL_SECTOR;
                newFile->diskInfo.parentCluster = NULL_CLUSTER;
                newFile->diskInfo.childCluster = NULL_CLUSTER;
                newFile->diskInfo.nextCluster = NULL_CLUSTER;
                newFile->diskInfo.prevCluster = NULL_CLUSTER;

                if (sendInfoToHD())
                {
                    //TODO: send the data to hard disk
                    ret = createFileIntoHardDrive(parent, newFile, NULL);
                }

                ret = createNewFpool(NULL, newFile, false, parent); 
            }
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
