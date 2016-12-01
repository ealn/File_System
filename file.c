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

#define CHUNK_SIZE    512

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

static void reallocData(File * pFile, uint32_t newSize)
{
    if (pFile != NULL)
    {
        pFile->data = (char *)MEMREALLOC(pFile->data,
                                         pFile->size,
                                         newSize);
    }
}

static void changeFileSize(File *pFile, uint32_t newSize)
{
    if (pFile != NULL)
    {
        reallocData(pFile, newSize);
        pFile->size = newSize;
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
                   pFile->size,
                   pFile->m_date,
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
            if (!pFile->is_opened)
            {
                pFile->is_opened = true; 
            }
            else
            {
                ret = FILE_IS_ALREADY_OPENED;
            }
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
            if (pFile->is_opened)
            {
                pFile->is_opened = false; 
            }
            else
            {
                ret = FILE_IS_ALREADY_CLOSED;
            }
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
            index = pFile->w_point;

            while (index + numberOfBytes > (pFile->size - 1))  //keep the \0 terminator
            {
                changeFileSize(pFile, pFile->size + CHUNK_SIZE);
            }
            
            memcpy(&pFile->data[index], input, numberOfBytes);

            pFile->w_point += numberOfBytes;

            getTimeStamp(pFile->m_date);

            /*TODO:
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
            //output = readDataFromFile(pFile);
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
            *output = pFile->r_point;
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
            if (newPoint < pFile->size)
            {
                pFile->r_point = newPoint;
            }
            else
            {
                ret = FAIL;
            }
        }
    }
    else
    {
        ret = FAIL;
    }

    return ret; 
}

static int32_t updateFileData(File *pFile, char *data, uint32_t size, uint32_t wPoint)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL
        && pFile->data != NULL
        && data != NULL
        && (strcmp(pFile->data, data) != 0))
    {
        MEMFREE((void *)pFile->data);

        pFile->data = (char *)MEMALLOC(sizeof(char)* size);
        strcpy(pFile->data, data);

        pFile->size = size;
        pFile->w_point = wPoint;

        if (sendInfoToHD())
        {
            ret = updateFileIntoHardDrive(pFile);
        }
    }

    return ret;
}

static int32_t updateFileCreationDate(File *pFile, char *newCreationDate)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL)
    {
        if (newCreationDate != NULL
            && (strlen(newCreationDate) == strlen(pFile->c_date)))
        {
            strcpy(pFile->c_date, newCreationDate);
        }
        else
        {
            //update from the system
            getTimeStamp(pFile->c_date); 
        }

        if (sendInfoToHD())
        {
            ret = updateFileIntoHardDrive(pFile);
        }
    }

    return ret;
}

int32_t updateFileModificationDate(File *pFile, char *newModDate)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL)
    {
        if (newModDate != NULL
            && (strcmp(pFile->m_date, newModDate) != 0)
            && (strlen(newModDate) == strlen(pFile->m_date)))
        {
            strcpy(pFile->m_date, newModDate);
        }
        else
        {
            //update from the system
            getTimeStamp(pFile->m_date); 
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
        && pFile->owner != NULL
        && newOwner != NULL
        && (strcmp(pFile->owner, newOwner) != 0))
    {
        uint32_t len = 0;

        len = strlen(newOwner) + 1;

        MEMFREE((void *)pFile->owner);

        pFile->owner = (char *)MEMALLOC(sizeof(char)* len);
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
            updateFileData(newFile, srcFile->data, srcFile->size, srcFile->w_point);
            updateFileCreationDate(newFile, srcFile->c_date);
            updateFileModificationDate(newFile, srcFile->m_date);
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
                newFile->name =  (char *)MEMALLOC(sizeof(char)* nameSize);
                newFile->data =  (char *)MEMALLOC(sizeof(char)* CHUNK_SIZE);
                newFile->owner = (char *)MEMALLOC(sizeof(char)* curUserSize);
                newFile->size = CHUNK_SIZE;
                //TODO: send permissions as parameter
                newFile->permissions = READ_ONLY | WRITE_ONLY | EXEC_ONLY;

                strcpy(newFile->name, pName);
                strcpy(newFile->owner, currentUser);

                getTimeStamp(newFile->c_date);
                getTimeStamp(newFile->m_date);

                ret = createNewFpool(NULL, newFile, false, parent);

                if (ret == SUCCESS
                    && sendInfoToHD())
                {
                    ret = createFileIntoHardDrive(newFile);
                }
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
        ret = removeFileOrFolderFromPool(pFile, NULL, false);

        if (ret == SUCCESS
            && sendInfoToHD())
        {
            ret = removeFileIntoHardDrive(pFile);
        }

        if (ret == SUCCESS)
        {
            if (pFile->data != NULL) 
            {
                MEMFREE((void *)pFile->data);
            }

            if (pFile->name != NULL)
            {
                MEMFREE((void *)pFile->name); 
            }

            if (pFile->owner != NULL)
            {
                MEMFREE((void *)pFile->owner);
            }

            freeFile(pFile);
        }
    }
    else
    {
        ret = FAIL;
    }

    return ret;
}
