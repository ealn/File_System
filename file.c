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

void printFileInfo(File *pFile)
{
    if (pFile != NULL)
    {
        printf("%s\n", pFile->name);
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

File * createNewFile(Folder *parent, char *pName, uint16_t permissions)
{
    File * newFile = NULL;
    uint32_t nameSize = 0;
    int32_t  ret = 0;

    if (pName != NULL)
    {
        ret = searchFileOrFolderIntoPool(parent, pName, NULL, NULL, true);

        if (ret == FILE_NOT_FOUND)
        {
            newFile = allocFile(); 
            nameSize = strlen(pName) + 1;  //add the \0 character

            if (newFile != NULL)
            {
                newFile->name = (char *)MEMALLOC(sizeof(char)* nameSize);
                newFile->data = (char *)MEMALLOC(sizeof(char)* CHUNK_SIZE);
                newFile->size = CHUNK_SIZE;
                newFile->permissions = permissions;

                memcpy(newFile->name, pName, nameSize);

                getTimeStamp(newFile->c_date);

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

            if (pFile->name)
            {
                MEMFREE((void *)pFile->name); 
            }

            freeFile(pFile);
        }
    }

    return ret;
}
