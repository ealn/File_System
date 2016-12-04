/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains all functions to handle the hard drive
 */

#include "defines.h"
#include <stdio.h>

#ifndef __HARD_DRIVE_H__
#define __HARD_DRIVE_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

//defines
#define NULL_SECTOR                 -1
#define NULL_CLUSTER                -1
#define INVALID_ADDRESS             -1
#define MAX_DATA_PER_SECTOR         64

//Typedefs
typedef union _FileFolder
{
    File   file;
    Folder folder;
}FileFolder;

typedef struct _MasterBootRecord
{
    uint32_t   numberOfClusters;
    uint32_t   numberOfDataSectors;
    uint32_t   numberOfClustersUsed;
    uint32_t   numberOfDataSectorsUsed;
    uint32_t   clusterAddress;
    uint32_t   dataSectorsAddress;
}MasterBootRecord;

typedef struct _Cluster
{
    uint8_t    isUsed;
    uint8_t    isDir;
    FileFolder fileFolder;
}Cluster;

typedef struct _DataSector
{
    uint8_t   isUsed;
    uint32_t  dataLenght;
    int32_t   nextDataSector;
    int32_t   prevDataSector;
    char      data[MAX_DATA_PER_SECTOR];
}DataSector;

//Functions

int32_t closeHardDrive(void);
int32_t initHardDrive(void);
int32_t insertFileIntoHD(Folder *parentFolder, File *pFile, const char *data);
int32_t insertFolderIntoHD(Folder *parentFolder, Folder *pFolder);
int32_t removeFileIntoHD(File *pFile);
int32_t removeFolderIntoHD(Folder *pFolder);
int32_t modifyFileIntoHD(File *pFile);
int32_t modifyFolderIntoHD(Folder *pFolder);
int32_t writeFileIntoHD(File *pFile, char * newData);
char * readFileFromHD(File *pFile);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__HARD_DRIVE_H__)
