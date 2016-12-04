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

/***************************** Hard Drive **************************************
 
|------------------------------------------------------------------------------|
|                         Master Boot Record                                   |
|------------------------------------------------------------------------------|
| Cluster1 | Cluster2 | Cluster3| ...                                          |
|-------------------------------                                               |
|.                                                                             |
|.                                                                             |
|.                                                                             |
|------------------------------------------------------------------------------|
| DataSector1 | DataSector2 | DataSector3 | ...                                |
|-----------------------------------------                                     |
|.                                                                             |
|.                                                                             |
|.                                                                             |
|------------------------------------------------------------------------------|
 
**/ 
 
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

void initDataSector(DataSector *pDataSector)
{
    if (pDataSector != NULL)
    {
        memset(pDataSector, 0, DATA_SECTOR_SIZE);

        //set default parameters
        pDataSector->nextDataSector = NULL_SECTOR;
        pDataSector->prevDataSector = NULL_SECTOR;
    }
}

void initCluster(Cluster * pCluster)
{
    if (pCluster != NULL)
    {
        DiskInfo  *diskInfo;

        memset(pCluster, 0, CLUSTER_SIZE);

        //set default parameters
        diskInfo = &(pCluster->fileFolder.file.diskInfo);

        diskInfo->cluster = NULL_CLUSTER;
        diskInfo->dataSector = NULL_SECTOR;
        diskInfo->parentCluster = NULL_CLUSTER;
        diskInfo->childCluster = NULL_CLUSTER;
        diskInfo->nextCluster = NULL_CLUSTER;
        diskInfo->prevCluster = NULL_CLUSTER;
    }
}

int32_t getChildCluster(Cluster *pCluster)
{
    int32_t clusterIndex = NULL_CLUSTER;

    if (pCluster != NULL)
    {
        if (pCluster->isDir)
        {
            clusterIndex = pCluster->fileFolder.folder.diskInfo.childCluster;
        }
        else
        {
            clusterIndex = pCluster->fileFolder.file.diskInfo.childCluster;
        }
        
    }

    return clusterIndex;
}

int32_t getParentCluster(Cluster *pCluster)
{
    int32_t clusterIndex = NULL_CLUSTER;

    if (pCluster != NULL)
    {
        if (pCluster->isDir)
        {
            clusterIndex = pCluster->fileFolder.folder.diskInfo.parentCluster;
        }
        else
        {
            clusterIndex = pCluster->fileFolder.file.diskInfo.parentCluster;
        }
        
    }

    return clusterIndex;
}

int32_t getPrevCluster(Cluster *pCluster)
{
    int32_t clusterIndex = NULL_CLUSTER;

    if (pCluster != NULL)
    {
        if (pCluster->isDir)
        {
            clusterIndex = pCluster->fileFolder.folder.diskInfo.prevCluster;
        }
        else
        {
            clusterIndex = pCluster->fileFolder.file.diskInfo.prevCluster;
        }
        
    }

    return clusterIndex;
}

int32_t getNextCluster(Cluster *pCluster)
{
    int32_t clusterIndex = NULL_CLUSTER;

    if (pCluster != NULL)
    {
        if (pCluster->isDir)
        {
            clusterIndex = pCluster->fileFolder.folder.diskInfo.nextCluster;
        }
        else
        {
            clusterIndex = pCluster->fileFolder.file.diskInfo.nextCluster;
        }
        
    }

    return clusterIndex;
}

void setChildCluster(Cluster *pCluster, int32_t child)
{
    if (pCluster != NULL)
    {
        if (pCluster->isDir)
        {
            pCluster->fileFolder.folder.diskInfo.childCluster = child;
        }
        else
        {
            pCluster->fileFolder.file.diskInfo.childCluster = child;
        }
    }
}

void setParentCluster(Cluster *pCluster, int32_t parent)
{
    if (pCluster != NULL)
    {
        if (pCluster->isDir)
        {
            pCluster->fileFolder.folder.diskInfo.parentCluster = parent;
        }
        else
        {
            pCluster->fileFolder.file.diskInfo.parentCluster = parent;
        }
    }
}

void setNextCluster(Cluster *pCluster, int32_t next)
{
    if (pCluster != NULL)
    {
        if (pCluster->isDir)
        {
            pCluster->fileFolder.folder.diskInfo.nextCluster = next;
        }
        else
        {
            pCluster->fileFolder.file.diskInfo.nextCluster = next;
        }
    }
}

void setPrevCluster(Cluster *pCluster, int32_t prev)
{
    if (pCluster != NULL)
    {
        if (pCluster->isDir)
        {
            pCluster->fileFolder.folder.diskInfo.prevCluster = prev;
        }
        else
        {
            pCluster->fileFolder.file.diskInfo.prevCluster = prev;
        }
    }
}

int32_t getClusterAddressAtIndex(int32_t index)
{
    int32_t address = INVALID_ADDRESS;

    if (g_masterBootRecord != NULL
        && (index < g_masterBootRecord->numberOfClusters))
    {
        address = g_masterBootRecord->clusterAddress + CLUSTER_SIZE * index;
    }

    return address;
}

int32_t getDataSectorAddressAtIndex(int32_t index)
{
    int32_t address = INVALID_ADDRESS;

    if (g_masterBootRecord != NULL
        && (index < g_masterBootRecord->numberOfDataSectors))
    {
        address = g_masterBootRecord->dataSectorsAddress + DATA_SECTOR_SIZE * index;
    }

    return address;
}

void writeHD(char *buff, uint32_t address, uint32_t size)
{
    if (g_hardDisk != NULL)
    {
        fseek(g_hardDisk, address, SEEK_SET);
        fwrite((void *)buff, 1, size, g_hardDisk); 
    }
}

void readHD(char *outputBuff, uint32_t address, uint32_t size)
{
    if (g_hardDisk != NULL)
    {
        fseek(g_hardDisk, address, SEEK_SET);
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

    //open file or create if it does not exist
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

    if (g_hardDisk != NULL
        && g_masterBootRecord != NULL)
    {
        char        buff[DEFAULT_HARD_DISK_SIZE];
        Cluster     cluster;
        DataSector  dataSector;
        uint32_t    i = 0;
        uint32_t    address = 0;

        //set to 0 the buffer
        memset(buff, 0, sizeof(char)*DEFAULT_HARD_DISK_SIZE);
        initCluster(&cluster);
        initDataSector(&dataSector);

        //write the buffer into hard drive
        writeHD(buff, 0, DEFAULT_HARD_DISK_SIZE);

        setDefaultMasterBootRecord();
        writeMasterBootRecordIntoHD();

        //Write clusters
        for (i = 0; i < g_masterBootRecord->numberOfClusters; i++)
        {
            address = getClusterAddressAtIndex(i);

            if (address != INVALID_ADDRESS)
            {
                writeHD((char *)&cluster, address, CLUSTER_SIZE); 
            }
        }

        //Write data sectors
        for (i = 0; i < g_masterBootRecord->numberOfDataSectors; i++)
        {
            address = getDataSectorAddressAtIndex(i);

            if (address != INVALID_ADDRESS)
            {
                writeHD((char *)&dataSector, address, DATA_SECTOR_SIZE); 
            }
        }
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

        if (ret == SUCCESS)
        {
            createRootFolder();
        }
    }

    return ret; 
}

int32_t getDataSectorAtIndex(int32_t index, DataSector * pOutputDataSector, int32_t *pOutputAddress)
{
    int32_t dataSector = NULL_SECTOR;

    if (index != NULL_SECTOR
        && pOutputDataSector != NULL
        && g_masterBootRecord != NULL
        && (index < g_masterBootRecord->numberOfDataSectors))
    {
        int32_t address = INVALID_ADDRESS;

        address = getDataSectorAddressAtIndex(index);

        if (pOutputAddress != NULL)
        {
            //send output address
            *pOutputAddress = address;
        }

        if (address != INVALID_ADDRESS)
        {
            readHD((char *)pOutputDataSector, address, DATA_SECTOR_SIZE); 
            dataSector = index;
        }
    }

    return dataSector; 
}

int32_t getFreeDataSector(void)
{
    int32_t freeDataSector = NULL_SECTOR;

    if (g_hardDisk != NULL
        && g_masterBootRecord != NULL)
    {
        int32_t   ret = 0;
        uint32_t  i = 0;
        DataSector dataSector;

        for (i = 0; i < g_masterBootRecord->numberOfDataSectors; i++)
        {
            ret = getDataSectorAtIndex(i, &dataSector, NULL);

            if (ret != NULL_SECTOR
                && dataSector.isUsed == 0)
            {
                freeDataSector = i;
                break;
            }
        }
    }

    return freeDataSector;
}

int32_t getClusterAtIndex(int32_t index, Cluster * pOutputCluster, int32_t *pOutputAddress)
{
    int32_t cluster = NULL_CLUSTER;

    if (index != NULL_CLUSTER
        && pOutputCluster != NULL
        && g_masterBootRecord != NULL
        && (index < g_masterBootRecord->numberOfClusters))
    {
        int32_t address = INVALID_ADDRESS;

        address = getClusterAddressAtIndex(index);

        if (pOutputAddress != NULL)
        {
            //send output address
            *pOutputAddress = address;
        }

        if (address != INVALID_ADDRESS)
        {
            readHD((char *)pOutputCluster, address, CLUSTER_SIZE); 
            cluster = index;
        }
    }

    return cluster; 
}

int32_t getFreeCluster(void)
{
    int32_t freeCluster = NULL_CLUSTER;

    if (g_hardDisk != NULL
        && g_masterBootRecord != NULL)
    {
        int32_t   ret = 0;
        uint32_t  i = 0;
        Cluster   cluster;

        for (i = 0; i < g_masterBootRecord->numberOfClusters; i++)
        {
            ret = getClusterAtIndex(i, &cluster, NULL);

            if (ret != NULL_CLUSTER
                && cluster.isUsed == 0)
            {
                freeCluster = i;
                break;
            }
        }
    }

    return freeCluster;
}

void freeDataSector(int32_t index)
{
    if (index != NULL_SECTOR)
    {
        DataSector dataSector;
        DataSector emptyDataSector;
        int32_t    ret = 0;
        int32_t    address = INVALID_ADDRESS;

        initDataSector(&emptyDataSector);
        ret = getDataSectorAtIndex(index, &dataSector, &address);

        if (ret != NULL_SECTOR
            && address != INVALID_ADDRESS)
        {
            //clean cluster
            writeHD((char *)&emptyDataSector, address, DATA_SECTOR_SIZE);
        }

    }
}

void freeLinkDataSector(int32_t firstIndex)
{
    if (firstIndex != NULL_SECTOR)
    {
        DataSector dataSector;
        DataSector nextDataSector;
        int32_t    ret = NULL_SECTOR;
        int32_t    lastSectorIndex = NULL_SECTOR;

        ret = getDataSectorAtIndex(firstIndex, &dataSector, NULL);

        if (ret != NULL_SECTOR)
        {
            //TODO
        }
    }
}

void unlinkCluster(Cluster * pCluster)
{
    if (pCluster != NULL)
    {
        Cluster   nextCluster;
        Cluster   prevCluster;
        Cluster   parentCluster;
        int32_t   prevClusterIndex = 0;
        int32_t   nextClusterIndex = 0;
        int32_t   parentClusterIndex = 0;
        int32_t   parentAddress = 0;
        int32_t   nextAddress = 0;
        int32_t   prevAddress = 0;
        int32_t   clusterIndex = 0;
        DiskInfo *pDiskInfo = NULL;

        if (pCluster->isDir)
        {
            pDiskInfo = &(pCluster->fileFolder.folder.diskInfo);
        }
        else
        {
            pDiskInfo = &(pCluster->fileFolder.file.diskInfo);
        }

        clusterIndex = pDiskInfo->cluster;

        parentClusterIndex = getClusterAtIndex(pDiskInfo->parentCluster, 
                                               &parentCluster, 
                                               &parentAddress);
        prevClusterIndex = getClusterAtIndex(pDiskInfo->prevCluster, 
                                             &prevCluster, 
                                             &prevAddress); 
        nextClusterIndex = getClusterAtIndex(pDiskInfo->nextCluster, 
                                             &nextCluster, 
                                             &nextAddress);

        if (parentClusterIndex != NULL_CLUSTER)
        {
            if (getChildCluster(&parentCluster) == clusterIndex)
            {
                //if there is more elements to the rigth
                if (nextClusterIndex != NULL_CLUSTER)
                {
                    setChildCluster(&parentCluster, nextClusterIndex);
                    setPrevCluster(&nextCluster, NULL_CLUSTER);
                }
                else
                {
                    setChildCluster(&parentCluster, NULL_CLUSTER);
                }
            }
            else
            {
                if (nextClusterIndex != NULL_CLUSTER)
                {
                    setPrevCluster(&nextCluster, prevClusterIndex);
                }
                if (prevClusterIndex != NULL_CLUSTER) 
                {
                    setNextCluster(&prevCluster, nextClusterIndex);
                }
            }
        }

        if (parentClusterIndex != NULL_CLUSTER)
        {
            writeHD((char *)&parentCluster, parentAddress, CLUSTER_SIZE); 
        }
        if (prevClusterIndex != NULL_CLUSTER)
        {
            writeHD((char *)&prevCluster, prevAddress, CLUSTER_SIZE); 
        }
        if (nextClusterIndex != NULL_CLUSTER)
        {
            writeHD((char *)&nextCluster, nextAddress, CLUSTER_SIZE); 
        }
    }
}

void freeCluster(int32_t index)
{
    if (index != NULL_CLUSTER)
    {
        Cluster cluster;
        Cluster emptyCluster;
        int32_t address = INVALID_ADDRESS;
        int32_t ret = 0;
        DiskInfo  *diskInfo = NULL;

        initCluster(&emptyCluster);
        ret = getClusterAtIndex(index, &cluster, &address);

        if (ret != NULL_CLUSTER
            && address != INVALID_ADDRESS)
        {
            //get DiskInfo
            diskInfo = &(cluster.fileFolder.file.diskInfo);

            if (diskInfo->dataSector != NULL_SECTOR)
            {
                freeLinkDataSector(diskInfo->dataSector);
            }

            unlinkCluster(&cluster);

            //clean cluster
            writeHD((char *)&emptyCluster, address, CLUSTER_SIZE);
            g_masterBootRecord->numberOfClustersUsed--;
        }
    }
}

int32_t createCluster(int32_t parentCluster, int32_t cluster)
{
    int32_t ret = SUCCESS;
    //TODO

    return ret;
}

int32_t createDataSector(int32_t prevDataSector, int32_t dataSector)
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

int32_t readClusterRecursive(int32_t clusterNumber, bool createFiles)
{
    int32_t ret = SUCCESS;

    if (clusterNumber != NULL_CLUSTER)
    {
        //TODO
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
        ret = readClusterRecursive(0, true);
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
        //TODO: Quitar el false del if
        //if (existHardDrive())
        if (false)
        {
            setSetInfoToHD(false);
            ret = openHardDriveFile();

            if (ret == SUCCESS)
            {
                readHardDriveInfo();
                setSetInfoToHD(true);
            }
        }
        else
        {
            setSetInfoToHD(true);
            ret = createHardDrive();
        }
    }
    else
    {
        ret = HD_ERROR_MASTER_BOOT_RECORD;
    }

    return ret;
}

int32_t insertFileIntoHD(Folder *parentFolder, File *pFile, const char *data)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL)
    {
        Folder    *lastFolder = NULL;
        File      *lastFile = NULL;
        DiskInfo  *diskInfo = NULL;
        DiskInfo  *parentDiskInfo = NULL;
        Cluster    cluster;
        int32_t    clusterIndex = 0;
        int32_t    address = 0;

        clusterIndex = getFreeCluster();
        diskInfo = &(pFile->diskInfo);

        if (parentFolder != NULL)
        {
            parentDiskInfo = &(parentFolder->diskInfo);
        }

        if (clusterIndex != NULL_CLUSTER) 
        {
            ret = getClusterAtIndex(clusterIndex, &cluster, &address);

            if (address != INVALID_ADDRESS)
            {
                cluster.isUsed = 1;
                diskInfo->cluster = clusterIndex;

                if (data != NULL)
                {
                    //TODO: create Data Sectors
                }
                if (parentDiskInfo != NULL)
                {
                    diskInfo->parentCluster = parentDiskInfo->cluster;

                    if (getNumberOfChilds(parentFolder) == 0)
                    {
                        parentDiskInfo->childCluster = diskInfo->cluster;
                    }
                    else
                    {
                        getLastElementOfFolder(parentFolder, &lastFile, &lastFolder); 

                        if (lastFile != NULL)
                        {
                            diskInfo->prevCluster = lastFile->diskInfo.cluster;
                            lastFile->diskInfo.nextCluster = diskInfo->cluster;
                        }
                        else if (lastFolder != NULL)
                        {
                            diskInfo->prevCluster = lastFolder->diskInfo.cluster;
                            lastFolder->diskInfo.nextCluster = diskInfo->cluster;
                        }
                    }
                }

                memcpy(&cluster.fileFolder.file, pFile, sizeof(File));

                writeHD((char *)&cluster, address, CLUSTER_SIZE);

                g_masterBootRecord->numberOfClustersUsed++;
            }
        }
    }

    return ret;
}

int32_t insertFolderIntoHD(Folder *parentFolder, Folder *pFolder)
{
    int32_t ret = SUCCESS;

    if (pFolder != NULL)
    {
        Folder    *lastFolder = NULL;
        File      *lastFile = NULL;
        DiskInfo  *diskInfo = NULL;
        DiskInfo  *parentDiskInfo = NULL;
        Cluster    cluster;
        int32_t    clusterIndex = 0;
        int32_t    address = 0;

        clusterIndex = getFreeCluster();
        diskInfo = &(pFolder->diskInfo);

        if (parentFolder != NULL)
        {
            parentDiskInfo = &(parentFolder->diskInfo);
        }

        if (clusterIndex != NULL_CLUSTER) 
        {
            ret = getClusterAtIndex(clusterIndex, &cluster, &address);

            if (address != INVALID_ADDRESS)
            {
                cluster.isUsed = 1;
                cluster.isDir = 1;
                diskInfo->cluster = clusterIndex;

                if (parentDiskInfo != NULL)
                {
                    diskInfo->parentCluster = parentDiskInfo->cluster;

                    if (getNumberOfChilds(parentFolder) == 0)
                    {
                        parentDiskInfo->childCluster = diskInfo->cluster;
                    }
                    else
                    {
                        getLastElementOfFolder(parentFolder, &lastFile, &lastFolder); 

                        if (lastFile != NULL)
                        {
                            diskInfo->prevCluster = lastFile->diskInfo.cluster;
                            lastFile->diskInfo.nextCluster = diskInfo->cluster;
                        }
                        else if (lastFolder != NULL)
                        {
                            diskInfo->prevCluster = lastFolder->diskInfo.cluster;
                            lastFolder->diskInfo.nextCluster = diskInfo->cluster;
                        }
                    }
                }

                memcpy(&cluster.fileFolder.folder, pFolder, sizeof(Folder));

                writeHD((char *)&cluster, address, CLUSTER_SIZE);

                g_masterBootRecord->numberOfClustersUsed++;
            }
        }
    }

    return ret;
}

int32_t removeFileIntoHD(File *pFile)
{
    int32_t ret = SUCCESS;

    if (pFile != NULL)
    {
        DiskInfo  *diskInfo = NULL;

        diskInfo = &(pFile->diskInfo);

        if (diskInfo->dataSector != NULL_SECTOR)
        {
            freeLinkDataSector(diskInfo->dataSector);
        }

        freeCluster(diskInfo->cluster);
    }

    return ret;
}

int32_t removeFolderIntoHD(Folder *pFolder)
{
    int32_t ret = SUCCESS;

    if (pFolder != NULL)
    {
        DiskInfo  *diskInfo = NULL;

        diskInfo = &(pFolder->diskInfo);

        freeCluster(diskInfo->cluster);
    }

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

