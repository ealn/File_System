/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Implementation of hard drive
 */
#include <string.h>

#include "hard_drive.h"
#include "file_system.h"
#include "folder.h"
#include "file.h"
#include "memutils.h"

#define HARD_DISK_NAME              "virtualHD.dat"
#define ATTR_FILE_READ              "r"
#define ATTR_FILE_READ_WRITE        "w+"

#define DEFAULT_CLUSTERS            512
#define DEFAULT_DATA_SECTORS        512
#define CLUSTER_SIZE                sizeof(Cluster)
#define DATA_SECTOR_SIZE            sizeof(DataSector)
#define MASTER_BOOT_RECORD_SIZE     sizeof(MasterBootRecord)

#define DEFAULT_HARD_DISK_SIZE      MASTER_BOOT_RECORD_SIZE + \
                                    CLUSTER_SIZE * DEFAULT_CLUSTERS + \
                                    DATA_SECTOR_SIZE * DEFAULT_DATA_SECTORS

static FILE             * g_hardDisk = NULL;
static MasterBootRecord * g_masterBootRecord = NULL;

void allocMasterBootRecord(void)
{
    if (g_masterBootRecord == NULL)
    {
        g_masterBootRecord = (MasterBootRecord *)MEMALLOC(MASTER_BOOT_RECORD_SIZE);
    }
}

void freeMasterBootRecord(void)
{
    if (g_masterBootRecord != NULL)
    {
        MEMFREE((void *)g_masterBootRecord);
    }
}

void writeHD(char *buff, uint32_t pos, uint32_t size)
{
    if (g_hardDisk != NULL)
    {
        fseek(g_hardDisk, pos, SEEK_SET);
        fwrite((void *)buff, 1, size, g_hardDisk); 
    }
}

void readHD(char *outputBuff, uint32_t pos, uint32_t size)
{
    if (g_hardDisk != NULL)
    {
        fseek(g_hardDisk, pos, SEEK_SET);
        fread((void *)outputBuff, 1, size, g_hardDisk); 
    }
}

bool existHardDrive(void)
{
    bool   ret = false;
    FILE * pFile = NULL;

    pFile = fopen(HARD_DISK_NAME, ATTR_FILE_READ);

    if (pFile != NULL)
    {
        fclose(pFile);
        ret = true;
    }

    return ret; 
}

int32_t openHardDriveFile(void)
{
    int32_t ret = SUCCESS;
    FILE  * pFile = NULL;

    pFile = fopen(HARD_DISK_NAME, ATTR_FILE_READ_WRITE);

    if (pFile != NULL)
    {
        g_hardDisk = pFile;
    }
    else
    {
        ret = HD_ERROR_OPENNING_HD_FILE;
    }

    return ret; 
}

int32_t writeMasterBootRecordIntoHD(void)
{
    int32_t ret = SUCCESS;

    if (g_hardDisk != NULL
        && g_masterBootRecord != NULL)
    {
        writeHD((char *)g_masterBootRecord, 0, MASTER_BOOT_RECORD_SIZE);
    }
    else
    {
        ret = FAIL;
    }

    return ret; 
}

void getMasterBootRecordFromHD(void)
{
    if (g_hardDisk != NULL
        && g_masterBootRecord != NULL)
    {
        readHD((char *)g_masterBootRecord, 0, MASTER_BOOT_RECORD_SIZE);
    }
}

void setDefaultMasterBootRecord(void)
{
    if (g_masterBootRecord != NULL)
    {
        g_masterBootRecord->numberOfClusters = DEFAULT_CLUSTERS;
        g_masterBootRecord->numberOfDataSectors = DEFAULT_DATA_SECTORS;
        g_masterBootRecord->numberOfClustersUsed = 0;
        g_masterBootRecord->numberOfDataSectorsUsed = 0;
        g_masterBootRecord->clusterAddress = MASTER_BOOT_RECORD_SIZE;
        g_masterBootRecord->dataSectorsAddress = MASTER_BOOT_RECORD_SIZE
                                                 + CLUSTER_SIZE * DEFAULT_CLUSTERS;
    }
}

int32_t formatHardDisk(void)
{
    int32_t ret = SUCCESS;

    if (g_hardDisk != NULL)
    {
        char buff[DEFAULT_HARD_DISK_SIZE];

        //set to 0 the buffer
        memset(buff, 0, sizeof(char)*DEFAULT_HARD_DISK_SIZE);

        //write the buffer into hard drive
        writeHD(buff, 0, DEFAULT_HARD_DISK_SIZE);

        setDefaultMasterBootRecord();
        writeMasterBootRecordIntoHD();
    }
    else
    {
        ret = HD_ERROR_FORMAT;
    }

    return ret;
}

int32_t createHardDrive(void)
{
    int32_t ret = SUCCESS;

    //create file if does not exist
    ret = openHardDriveFile();

    if (ret == SUCCESS
        && g_hardDisk != NULL)
    {
        ret = formatHardDisk();
    }

    return ret; 
}

int32_t getDataSectorAtIndex(int32_t index, DataSector * pOutputDataSector)
{
    int32_t dataSector = NULL_SECTOR;

    if (index != NULL_SECTOR
        && pOutputDataSector != NULL)
    {
        uint32_t address = 0;

        address = g_masterBootRecord->dataSectorsAddress + DATA_SECTOR_SIZE * index;

        readHD((char *)pOutputDataSector, address, DATA_SECTOR_SIZE);
    }

    return dataSector; 
}

int32_t getFreeDataSector(void)
{
    int32_t freeDataSector = NULL_SECTOR;

    if (g_hardDisk != NULL
        && g_masterBootRecord != NULL)
    {
        uint32_t  i = 0;

        for (i = 0; i < g_masterBootRecord->numberOfDataSectors; i++)
        {
            //TODO: search into hard drive
        }
    }

    return freeDataSector;
}

int32_t getClusterAtIndex(int32_t index, Cluster * pOutputCluster)
{
    int32_t cluster = NULL_CLUSTER;

    if (index != NULL_CLUSTER
        && pOutputCluster != NULL)
    {
        uint32_t address = 0;

        address = g_masterBootRecord->clusterAddress + CLUSTER_SIZE * index;

        readHD((char *)pOutputCluster, address, CLUSTER_SIZE);
    }

    return cluster; 
}

int32_t getFreeCluster(void)
{
    int32_t freeCluster = NULL_CLUSTER;

    if (g_hardDisk != NULL
        && g_masterBootRecord != NULL)
    {
        uint32_t  i = 0;

        for (i = 0; i < g_masterBootRecord->numberOfClusters; i++)
        {
            //TODO: search into hard drive
        }
    }

    return freeCluster;
}

void freeCluster(int32_t cluster)
{
    //TODO
}

void freeDataSector(int32_t dataSector)
{
    //TODO
}

int32_t writeDataSector(int32_t dataSector)
{
    int32_t ret = SUCCESS;

    //TODO

    return ret;
}

int32_t readDataSector(int32_t dataSector)
{
    int32_t ret = SUCCESS;

    //TODO

    return ret;
}

int32_t writeCluster(int32_t cluster)
{
    int32_t ret = SUCCESS;

    //TODO

    return ret;
}

int32_t readClusterRecursive(int32_t clusterNumber, bool createFiles)
{
    int32_t ret = SUCCESS;

    if (clusterNumber != NULL_CLUSTER)
    {
        //TODO
    }

    return ret; 
}

int32_t readCluster(int32_t clusterNumber, bool recursive, bool createFiles)
{
    int32_t ret = SUCCESS;

    if (clusterNumber != NULL_CLUSTER)
    {
        if (recursive) 
        {
            readClusterRecursive(clusterNumber, createFiles);
        }
        else
        {
            //TODO
        }
    }

    return ret;
}

int32_t readHardDriveInfo(void)
{
    int32_t ret = SUCCESS;

    if (g_masterBootRecord != NULL
        && g_hardDisk != NULL)
    {
        getMasterBootRecordFromHD(); 

        //read cluster
        ret = readCluster(0, true, true);
    }


    return ret;
}

/******************************** Entry points ************************************/

int32_t closeHardDrive(void)
{
    int32_t ret = SUCCESS;

    if (g_hardDisk != NULL)
    {
        fclose(g_hardDisk);
    }

    freeMasterBootRecord();

    return ret;
}

int32_t initHardDrive(void)
{
    int32_t ret = SUCCESS;

    allocMasterBootRecord();

    if (g_masterBootRecord != NULL)
    {
        if (existHardDrive()) 
        {
            ret = openHardDriveFile();

            if (ret == SUCCESS)
            {
                readHardDriveInfo(); 
            }
        }
        else
        {
            ret = createHardDrive();
        }
    }
    else
    {
        ret = HD_ERROR_MASTER_BOOT_RECORD;
    }

    return ret;
}

int32_t insertFileIntoHD(File *pFile)
{
    int32_t ret = SUCCESS;

    //TODO:

    return ret;
}

int32_t insertFolderIntoHD(Folder *pFolder)
{
    int32_t ret = SUCCESS;

    //TODO:

    return ret;
}

int32_t removeFileIntoHD(File *pFile)
{
    int32_t ret = SUCCESS;

    //TODO:

    return ret;
}

int32_t removeFolderIntoHD(Folder *pFolder)
{
    int32_t ret = SUCCESS;

    //TODO:

    return ret;
}

int32_t modifyFileIntoHD(File *pFile)
{
    int32_t ret = SUCCESS;

    //TODO:

    return ret;
}

int32_t modifyFolderIntoHD(Folder *pFolder)
{
    int32_t ret = SUCCESS;

    //TODO:

    return ret;
}

