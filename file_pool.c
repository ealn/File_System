/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Implementation of methods to handle the file pool
 */
#include <string.h>

#include "file_pool.h"
#include "memutils.h"
#include "trace.h"

//Structs
struct _FilePool
{
    File     * pFile;
    FilePool * next;
    FilePool * prev;
};

FilePool * g_filePool = NULL;
FilePool * g_currentPool = NULL;

//Defines

//Implementation
static FilePool * allocFilePool(void)
{
    FilePool * pFilePool = NULL;
    
    pFilePool = (FilePool *)MEMALLOC(sizeof(FilePool));
    
    if (pFilePool == NULL)
    {
        FILE_POOL_ERROR("File pool could not be allocated\n");
    }
    
    return pFilePool;
}

static void freeFilePool(FilePool *pFilePool)
{
    if (pFilePool != NULL)
    {
        MEMFREE((void *)pFilePool);
    }
    else
    {
        FILE_POOL_ERROR("File pool is null\n");
    }
}

static int32_t createNewEntryIntoFilePool(void)
{
    int32_t    ret = SUCCESS;
    FilePool * pNewFilePool = NULL;

    if (g_filePool != NULL
        && g_currentPool != NULL)
    {
        pNewFilePool = allocFilePool();

        if (pNewFilePool != NULL)
        {
            g_currentPool->next = pNewFilePool;
            pNewFilePool->prev = g_currentPool;

            g_currentPool = pNewFilePool;
        }
        else
        {
            FILE_POOL_ERROR("File pool could not be allocated\n");
            ret = FAIL;
        }
    }

    return ret;
}

static bool isEmptySlot(FilePool * pFilePool)
{
    bool ret = false;

    if (pFilePool != NULL
        && pFilePool->pFile == NULL)
    {
        ret = true;
    }

    return ret; 
}
//Entry points
int32_t createFilePool(void)
{
    int32_t ret = SUCCESS;

    if (g_filePool == NULL)
    {
        g_filePool = allocFilePool();

        if (g_filePool != NULL)
        {
            g_currentPool = g_filePool;
        }
        else 
        {
            FILE_POOL_ERROR("Global file pool could not be allocated\n");
            ret = FAIL;
        }
    }
    else
    {
        FILE_POOL_WARNING("Global file pool is not null\n");
        ret = FAIL;
    }

    return ret;
}

void destroyFilePool(void)
{
    if (g_filePool != NULL)
    {
        FilePool * pFilePool;
        FilePool * pNextFilePool;

        pFilePool = g_filePool;
        pNextFilePool = pFilePool->next;
        //TODO

    }
}

int32_t insertFileIntoPool(File * pFile)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL)
    {
        if (g_currentPool != NULL)
        {
            if (isEmptySlot(g_currentPool))
            {
                g_currentPool->pFile = pFile;
            }
            else
            {
                ret = createNewEntryIntoFilePool();

                if (ret == SUCCESS)
                {
                    g_currentPool->pFile = pFile;
                }
            }
        }
        else
        {
            FILE_POOL_ERROR("Current pool is null\n");
        }
    }
    else
    {
        FILE_POOL_ERROR("File is null\n");
        ret = FAIL;
    }

    return ret; 
}
