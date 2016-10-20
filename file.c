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

#define CHUNK_SIZE    20
//Structs
struct _File
{
    char *   name;
    char *   data;
    uint32_t size;
    uint32_t currentPos;
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

static void destroyFile(File * pFile)
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

//Entry points
int32_t createNewFile(char *pName)
{
    int32_t ret = SUCCESS;
    File   *pFile = NULL;

    if (pName != NULL)
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

    return ret; 
}

int32_t deleteFile(char *pName)
{
    int32_t ret = SUCCESS;

    if (pName != NULL)
    {

    }

    return ret; 
}

int32_t writeFile(char *pName)
{
    int32_t ret = SUCCESS;

    if (pName != NULL)
    {

    }

    return ret; 
}

int32_t readFile(char *pName)
{
    int32_t ret = SUCCESS;

    if (pName != NULL)
    {

    }

    return ret; 
}

int32_t tellFile(char *pName)
{
    int32_t ret = SUCCESS;

    if (pName != NULL)
    {

    }

    return ret; 
}

int32_t seekFile(char *pName)
{
    int32_t ret = SUCCESS;

    if (pName != NULL)
    {

    }

    return ret; 
}
