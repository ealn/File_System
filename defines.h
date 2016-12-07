/*
 * Copyright (c) 2016 by Efrain Adrian Luna Nevarez
 *                       Emmanuel Salcido Maldonado
 *                       Jesus Eduardo Silva Padilla
 *                       Efrain Arrambide Barron
 *                       Ricardo Isaac Gonzalez Ordaz
 *                       Victor Antonio Morales Carrillo
 * All Rights Reserved
 *
 * Authors: Efrain Adrian Luna Nevarez
 *          Emmanuel Salcido Maldonado
 *          Jesus Eduardo Silva Padilla
 *          Efrain Arrambide Barron
 *          Ricardo Isaac Gonzalez Ordaz
 *          Victor Antonio Morales Carrillo
 *
 * Porpuse: Header that contains the constants to detect errors
 */

#include <stdint.h>
#include <stdbool.h>

#include "trace.h"

#ifndef __DEFINES_H__
#define __DEFINES_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

// Defines
#define READ_ONLY                  0x0001
#define WRITE_ONLY                 0x0002
#define EXEC_ONLY                  0x0004

#define DEFAULT_PERMISSIONS        READ_ONLY       \
                                   | WRITE_ONLY    \
                                   | (READ_ONLY << 4) \
                                   | (WRITE_ONLY << 4)

#define EXIT                          1
#define SUCCESS                       0
#define FAIL                         -1
#define FILE_NOT_FOUND               -2
#define FILE_ALREADY_EXIST           -3
#define FILE_IS_ALREADY_OPENED       -4
#define FILE_IS_ALREADY_CLOSED       -5
#define THERE_ARE_NOT_FILES          -6
#define FOLDER_NOT_FOUND             -7
#define FOLDER_ALREADY_EXIST         -8
#define COMMAND_NOT_FOUND            -9
#define INVALID_PARAMETERS           -10
#define FILE_CAN_NOT_BE_DELETED      -11
#define FOLDER_CAN_NOT_BE_DELETED    -12
#define FILE_CAN_NOT_BE_OVERWRITTEN  -13
#define INVALID_PASSWORD             -14
#define INVALID_USER                 -15
#define INVALID_PERMISSIONS          -16

#define HD_ERROR_OPENNING_HD_FILE            -50
#define HD_ERROR_MASTER_BOOT_RECORD          -51
#define HD_ERROR_FORMAT                      -52
#define HD_ERROR_THERE_IS_NOT_CLUSTER_AVAIL  -53
#define HD_ERROR_THERE_IS_NOT_DATA_SEC_AVAIL -54

#define PERM_BUF_SIZE   8

#define MAX_F_NAME_SIZE      64
#define MAX_OWNER_SIZE       12
#define MAX_DATE_SIZE        20


// Type definitions
typedef struct _Folder    Folder;
typedef struct _File      File;
typedef struct _Fpool     Fpool;
typedef struct _DiskInfo  DiskInfo;

struct _DiskInfo
{
    int32_t     cluster;        //Cluster where the file or folder is allocated
    int32_t     dataSector;     //Sector where the data is allocated
    uint32_t    dataSize;       //Data size
    int32_t     parentCluster;  //Cluster of parent folder
    int32_t     childCluster;   //Cluster of child
    int32_t     nextCluster;    //Cluster of next element
    int32_t     prevCluster;    //Cluster of previous element
};

struct _Fpool
{
    File    * file;
    Folder  * folder;
    Fpool   * parent;
    Fpool   * next;
    Fpool   * prev;
    Fpool   * child;
    Fpool   * last;
    bool      isDir;
    uint32_t  nElements;
};

struct _File
{
    char     name[MAX_F_NAME_SIZE];
    char     owner[MAX_OWNER_SIZE];
    uint16_t permissions;
    char     date[MAX_DATE_SIZE];        //modification date
    Fpool   *fpool;
    DiskInfo diskInfo;
};

struct _Folder
{
    char     name[MAX_F_NAME_SIZE];
    char     owner[MAX_OWNER_SIZE];
    uint16_t permissions;
    char     date[MAX_DATE_SIZE];    //creation date
    Fpool   *fpool;
    DiskInfo diskInfo;
};

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__DEFINES_H__)
