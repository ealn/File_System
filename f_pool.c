/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Implementation of methods to handle the file pool
 */
#include <string.h>

#include "f_pool.h"
#include "folder.h"
#include "file.h"
#include "file_system.h"
#include "memutils.h"

//Implementation
static Fpool * allocFPool(void)
{
    Fpool * pFPool = NULL;
    
    pFPool = (Fpool *)MEMALLOC(sizeof(Fpool));
    
    return pFPool;
}

static void freeFPool(Fpool *pFpool)
{
    if (pFpool != NULL)
    {
        MEMFREE((void *)pFpool);
    }
}

int32_t insertElementIntoPool(Folder *pParentFolder, Fpool *pFPool)
{
    Fpool *parentFPool = NULL;
    int32_t    ret = SUCCESS;

    if (pParentFolder != NULL)
    {
        parentFPool = pParentFolder->fpool;

        if (parentFPool != NULL)
        {
            pFPool->parent = parentFPool;

            //if the parent does not have elements
            if (parentFPool->nElements == 0)
            {
                parentFPool->child = pFPool;

                //update last child
                parentFPool->last = pFPool;
                parentFPool->nElements++;
            }
            else if (parentFPool->last != NULL)
            {
                //link new FPool with the last child
                parentFPool->last->next = pFPool;
                pFPool->prev = parentFPool->last;

                //update last child
                parentFPool->last = pFPool;
                parentFPool->nElements++;
            }
            else
            {
                //Never reach here
                freeFPool(pFPool);
                ret = FAIL;
            }
        }
    }

    return ret;
}

int32_t createNewFpool(Folder *pFolder, File *pFile, bool isDir, Folder *pParentFolder)
{
    int32_t    ret = SUCCESS;

    if (pFolder != NULL
        || pFile != NULL)
    {
        Fpool *pFPool = NULL;

        pFPool = allocFPool();

        if (pFPool != NULL)
        {
            pFPool->isDir = isDir;

            ret = insertElementIntoPool(pParentFolder, pFPool);

            if (isDir
                && pFolder != NULL)
            {
                pFPool->folder = pFolder;
                pFolder->fpool = pFPool;
            }
            else if (!isDir
                     && pFile != NULL)
            {
                pFPool->file = pFile;
                pFile->fpool = pFPool;
            }
            else
            {
                //Never reach here
                freeFPool(pFPool);
                ret = FAIL;
            }
        }
    }

    return ret;
}

int32_t searchFileOrFolderIntoPool(Folder     *parentFolder,
                                   const char *pName,
                                   File      **ppOutputFile,
                                   Folder    **ppOutputFolder,
                                   bool        searchDir)
{
    int32_t ret;
    bool found = false;
    bool selectParent = false;
    bool selectGrandParent = false;

    if (searchDir)
    {
        ret = FOLDER_NOT_FOUND;
    }
    else
    {
        ret = FILE_NOT_FOUND;
    }

    if (pName != NULL
        && parentFolder != NULL)
    {
        Fpool * parentFpool = NULL;
        Fpool * grandParent = NULL;

        parentFpool = parentFolder->fpool;

        if (parentFpool != NULL)
        {
            Fpool * pFPool = NULL;

            pFPool = parentFpool->child;
            grandParent = parentFpool->parent;

            // if the folder contains elements
            if (pFPool != NULL)
            {
                while (pFPool != NULL) 
                {
                    if (searchDir)
                    {
                        if (pFPool->folder != NULL
                            && (strcmp(pFPool->folder->name, pName) == 0))
                        {
                            found = true; 
                            break;
                        }
                        else if (strcmp(CURRENT_FOLDER, pName) == 0)
                        {
                            selectParent = true;
                            found = true;
                            break;
                        }
                        else if (strcmp(PARENT_FOLDER, pName) == 0) 
                        {
                            selectGrandParent = true;
                            found = true;
                            break;
                        }
                    }
                    else
                    {
                        if (pFPool->file != NULL
                            && (strcmp(pFPool->file->name, pName) == 0))
                        {
                            found = true;
                            break;
                        }
                    }

                    pFPool = pFPool->next;
                }
            }
            else
            {
                if (searchDir)
                {
                    if (strcmp(CURRENT_FOLDER, pName) == 0)
                    {
                        selectParent = true;
                        found = true;
                    }
                    else if (strcmp(PARENT_FOLDER, pName) == 0) 
                    {
                        selectGrandParent = true;
                        found = true;
                    }
                }
            }

            if (found)
            {
                // element found
                if (searchDir
                    && ppOutputFolder != NULL)
                {
                    if (selectParent)
                    {
                        *ppOutputFolder = parentFpool->folder;
                    }
                    else if (selectGrandParent)
                    {
                        //if the grand parent exist
                        if (grandParent != NULL)
                        {
                            *ppOutputFolder = grandParent->folder;
                        }
                        else
                        {
                            //if not then select the parentFolder
                            *ppOutputFolder = parentFpool->folder;
                        }
                    }
                    else
                    {
                        *ppOutputFolder = pFPool->folder; 
                    }
                }
                else if (!searchDir
                         && ppOutputFile != NULL) 
                {
                    *ppOutputFile = pFPool->file;
                }

                ret = SUCCESS; 
            }
        }
    }

    return ret; 
}

int32_t removeFileOrFolderFromPool(File * pFile, Folder *pFolder, bool isDir)
{
    int32_t ret = SUCCESS;

    if ((!isDir
         && pFile != NULL
         && pFile->fpool != NULL)
        || (isDir
            && pFolder != NULL
            && pFolder->fpool != NULL))
    {
        Fpool * pFPool = NULL;
        Fpool * parentFpool = NULL;
        Fpool * pPrev = NULL;
        Fpool * pNext = NULL;

        if (isDir)
        {
            pFPool = pFolder->fpool; 
        }
        else
        {
            pFPool = pFile->fpool;
        }

        parentFpool = pFPool->parent;

        if (parentFpool != NULL)
        {
            pPrev = pFPool->prev;
            pNext = pFPool->next;

            //if the file pool is the first child
            if (pFPool == parentFpool->child)
            {
                //if there are more elements to the right
                if (pNext != NULL)
                {
                    parentFpool->child = pNext;
                    pNext->prev = NULL;
                }
                else
                {
                    parentFpool->child = NULL;
                    parentFpool->last = NULL;
                }
            }
            else
            {
                if (pNext != NULL)
                {
                    pNext->prev = pPrev;
                }
                if (pPrev != NULL) 
                {
                    pPrev->next = pNext;

                    if (pFPool == parentFpool->last)
                    {
                        parentFpool->last = pPrev;
                    }
                }
            }

            parentFpool->nElements--;
            freeFPool(pFPool); 
        }
    }

    return ret; 
}

int32_t printInfoOfPool(Folder * pFolder, bool showDetails)
{
    int32_t ret = SUCCESS;

    if (pFolder != NULL
        && pFolder->fpool != NULL)
    {
        Fpool *pFPool = NULL; 

        pFPool = pFolder->fpool->child;

        while (pFPool != NULL)
        {
            if (pFolder->fpool->nElements > 0)
            {
                if (pFPool->isDir)
                {
                    printFolderInfo(pFPool->folder, showDetails);
                }
                else
                {
                    printFileInfo(pFPool->file, showDetails);
                }
            }
            else 
            {
                ret = THERE_ARE_NOT_FILES;
                break;
            }

            pFPool = pFPool->next; 
        }

    }

    return ret;
}

