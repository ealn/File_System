/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Implementation of methods to handle folders
 */

#include <string.h>

#include "folder.h"
#include "f_pool.h"
#include "memutils.h"
#include "trace.h"
#include "file_system.h"
#include "interface.h"

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

Folder * createNewFolder(Folder * parent, const char *pName, const char *pCreationDate)
{
    Folder * newFolder = NULL;
    uint32_t nameSize = 0;
    int32_t  ret = 0;

    if (pName != NULL)
    {
        ret = searchFileOrFolderIntoPool(parent, pName, NULL, NULL, true);

        if (ret == FILE_NOT_FOUND)
        {
            newFolder = allocFolder(); 
            nameSize = strlen(pName) + 1;  //add the \0 character

            if (newFolder != NULL)
            {
                newFolder->name = (char *)MEMALLOC(sizeof(char)* nameSize);
                strcpy(newFolder->name, pName);

                if (pCreationDate == NULL)
                {
                    getTimeStamp(newFolder->c_date); 
                }
                else
                {
                    memcpy(newFolder->c_date, pCreationDate, (TIME_BUF_SIZE - 1));
                }

                ret = createNewFpool(newFolder, NULL, true, parent);

                if (ret == SUCCESS
                    && sendInfoToHD())
                {
                    createFolderIntoHardDrive(newFolder);
                }
            }
        }
    }
    
    return newFolder;
}

int32_t destroyFolder(Folder * pFolder, bool recursive)
{
    int32_t ret = 0;

    if (pFolder != NULL)
    {
        if (sendInfoToHD())
        {
            removeFolderIntoHardDrive(pFolder);
        }
        if (pFolder->name != NULL) 
        {
            MEMFREE((void *)pFolder->name); 
        }

        //TODO implement recursive
        ret = removeFileOrFolderFromPool(NULL, pFolder, true);

        freeFolder(pFolder);
    }

    return ret;
}

void printFolderInfo(Folder * pFolder)
{
    if (pFolder != NULL)
    {
        printf("%s\n", pFolder->name);
    }
}


