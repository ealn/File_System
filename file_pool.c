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
#include "file.h"

//Structs
struct _FilePool
{
    File     * pFile;
    FilePool * next;
    FilePool * prev;
};

FilePool * g_filePool = NULL;
FilePool * g_lastPool = NULL;

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
        && g_lastPool != NULL)
    {
        pNewFilePool = allocFilePool();

        if (pNewFilePool != NULL)
        {
            g_lastPool->next = pNewFilePool;
            pNewFilePool->prev = g_lastPool;

            g_lastPool = pNewFilePool;
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
            g_lastPool = g_filePool;
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
        FilePool * pFilePool = NULL;

        while (g_lastPool != NULL)
        {
            pFilePool = g_lastPool->prev;

            if (g_lastPool->pFile != NULL)
            {
                destroyFile(g_lastPool->pFile);
            }

            freeFilePool(g_lastPool);

            g_lastPool = pFilePool;
        }
    }
}

int32_t insertFileIntoPool(File * pFile)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL)
    {
        if (g_lastPool != NULL)
        {
            if (isEmptySlot(g_lastPool))
            {
                g_lastPool->pFile = pFile;
            }
            else
            {
                ret = createNewEntryIntoFilePool();

                if (ret == SUCCESS)
                {
                    g_lastPool->pFile = pFile;
                }
            }
        }
        else
        {
            FILE_POOL_ERROR("Last pool is null\n");
        }
    }
    else
    {
        FILE_POOL_ERROR("File is null\n");
        ret = FAIL;
    }

    return ret; 
}

int32_t removeFileIntoPool(File * pFile)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL)
    {
        if (g_lastPool != NULL)
        {
            FilePool * pFilePool = NULL;
            FilePool * pPrev = NULL;
            FilePool * pNext = NULL;

            pFilePool = g_filePool;

            while (pFilePool != NULL)
            {
                if (pFilePool->pFile == pFile)
                {
                    if (pFilePool == g_filePool)
                    {
                        destroyFile(pFilePool->pFile);
                        pFilePool->pFile = NULL;
                    }
                    else
                    {
                        pPrev = pFilePool->prev;
                        pNext = pFilePool->next;

                        if (pNext != NULL)
                        {
                            pNext->prev = pPrev;
                        }
                        if (pPrev != NULL) 
                        {
                            pPrev->next = pNext;

                            if (pFilePool == g_lastPool)
                            {
                                g_lastPool = pPrev;
                            }
                        }

                        destroyFile(pFilePool->pFile); 
                        freeFilePool(pFilePool);
                    }

                    break;
                }
                else
                {
                    ret = FILE_NOT_FOUND;
                }

                pFilePool = pFilePool->next;
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

int32_t searchFileIntoPool(char *pName, File **ppOutputFile)
{
    int32_t ret = SUCCESS;

    if (pName != NULL
        && ppOutputFile != NULL)
    {
        if (g_filePool != NULL)
        {
            FilePool * pFilePool = NULL;

            pFilePool = g_filePool;

            while (pFilePool != NULL)
            {
                if (pFilePool->pFile != NULL
                    && (strcmp(getFileName(pFilePool->pFile), pName) == 0))
                {
                    *ppOutputFile = pFilePool->pFile;
                    ret = SUCCESS;
                    break;
                }
                else
                {
                    ret = FILE_NOT_FOUND;
                }

                pFilePool = pFilePool->next;
            }
        }
        else
        {
            FILE_POOL_ERROR("File pool is null\n");
        }
    }
    else
    {
        FILE_POOL_ERROR("Name is null or output file is null\n");
        ret = FAIL;
    }

    return ret; 
}
