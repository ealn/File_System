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
#include "file_pool.h"
#include "memutils.h"
#include "trace.h"

#define CHUNK_SIZE    512
//Structs
struct _File
{
    char *   name;
    char *   data;
    uint32_t size;
    uint32_t w_point;      //write point
    uint32_t r_point;      //read point
    bool     is_opened;
};

//Defines

//Implementation
static File * allocFile(void)
{
    File * pFile = NULL;
    
    pFile = (File *)MEMALLOC(sizeof(File));
    
    if (pFile == NULL)
    {
        FILE_ERROR("File could not be allocated\n");
    }
    
    return pFile;
}

static void freeFile(File *pFile)
{
    if (pFile != NULL)
    {
        MEMFREE((void *)pFile);
    }
    else
    {
        FILE_ERROR("File is null\n");
    }
}

static void reallocData(File * pFile, uint32_t newSize)
{
    if (pFile != NULL)
    {
        pFile->data = (char *)MEMREALLOC(pFile->data,
                                         pFile->size,
                                         newSize);

        if (pFile->data == NULL)
        {
            FILE_ERROR("Data could not be reallocated\n");
        }
    }
}

static File * createFile(char *pName)
{
    File * newFile = NULL;
    uint32_t nameSize = 0;

    if (pName != NULL)
    {
        newFile = allocFile(); 
        nameSize = strlen(pName) + 1;  //add the \0 character

        if (newFile != NULL)
        {
            newFile->name = (char *)MEMALLOC(sizeof(char)* nameSize);
            newFile->data = (char *)MEMALLOC(sizeof(char)* CHUNK_SIZE);
            newFile->size = CHUNK_SIZE;

            memcpy(newFile->name, pName, nameSize);
        }
    }
    
    return newFile;
}

void destroyFile(File * pFile)
{
    if (pFile != NULL)
    {
        if (pFile->data != NULL)
        {
            MEMFREE((void *)pFile->data);
            MEMFREE((void *)pFile->name);
            freeFile(pFile);
        }
        else
        {
            FILE_ERROR("File does not have data\n");
        }
    }
    else
    {
        FILE_WARNING("File is null\n");
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

char * getFileName(File *pFile)
{
    char * fileName = NULL;

    if (pFile != NULL)
    {
        fileName = pFile->name;
    }

    return fileName;
}

//Entry points
int32_t createNewFile(char *pName)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (pName != NULL)
    {
        ret = searchFileIntoPool(pName, &pFile);

        if (ret == FILE_NOT_FOUND)
        {
            pFile = createFile(pName); 

            if (pFile != NULL)
            {
                ret = insertFileIntoPool(pFile);
            }
            else
            {
                FILE_ERROR("File could not be created\n");
                ret = FAIL;
            }
        }
        else
        {
            ret = FILE_ALREADY_EXIST;
        }
    }
    else
    {
        FILE_ERROR("Name is null\n");
        ret = FAIL;
    }

    return ret; 
}

int32_t openFile(char *pName)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (pName != NULL)
    {
        ret = searchFileIntoPool(pName, &pFile);

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
        FILE_ERROR("Name is null\n");
        ret = FAIL;
    }

    return ret; 
}

int32_t closeFile(char *pName)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (pName != NULL)
    {
        ret = searchFileIntoPool(pName, &pFile);

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
        FILE_ERROR("Name is null\n");
        ret = FAIL;
    }

    return ret; 
}

int32_t deleteFile(char *pName)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (pName != NULL)
    {
        uint32_t index = 0;

        ret = searchFileIntoPool(pName, &pFile);

        if (pFile != NULL)
        {
            ret = removeFileIntoPool(pFile);
        }
    }
    else
    {
        FILE_ERROR("Name is null\n");
        ret = FAIL;
    }

    return ret; 
}

int32_t writeFile(char *pName, char *input, uint32_t numberOfBytes)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (pName != NULL
        && input != NULL
        && numberOfBytes >= 1)
    {
        uint32_t index = 0;

        ret = searchFileIntoPool(pName, &pFile);

        if (pFile != NULL)
        {
            index = pFile->w_point;

            while (index + numberOfBytes > (pFile->size - 1))  //keep the \0 terminator
            {
                changeFileSize(pFile, pFile->size + CHUNK_SIZE);
            }
            
            memcpy(&pFile->data[index], input, numberOfBytes);

            pFile->w_point += numberOfBytes;
        }
    }
    else
    {
        FILE_ERROR("Name is null or output is null\n");
        ret = FAIL;
    }

    return ret; 
}

int32_t readFile(char *pName, char *output, uint32_t numberOfBytes)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (pName != NULL
        && output != NULL)
    {
        uint32_t index = 0;

        ret = searchFileIntoPool(pName, &pFile);

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
        }
    }
    else
    {
        FILE_ERROR("Name is null or output is null\n");
        ret = FAIL;
    }

    return ret; 
}

int32_t tellFile(char *pName, uint32_t *output)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (pName != NULL
        && output != NULL)
    {
        ret = searchFileIntoPool(pName, &pFile);

        if (pFile != NULL)
        {
            *output = pFile->r_point;
        }
    }
    else
    {
        FILE_ERROR("Name is null or output is null\n");
        ret = FAIL;
    }

    return ret; 
}

int32_t seekFile(char *pName, uint32_t newPoint)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (pName != NULL)
    {
        ret = searchFileIntoPool(pName, &pFile);

        if (pFile != NULL)
        {
            if (newPoint < pFile->size)
            {
                pFile->r_point = newPoint;
            }
            else
            {
                FILE_WARNING("newPoint is major than size\n");
                ret = FAIL;
            }
        }
    }
    else
    {
        FILE_ERROR("Name is null\n");
        ret = FAIL;
    }

    return ret; 
}
