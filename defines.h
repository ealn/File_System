/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
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

// Type definitions
typedef struct _Folder Folder;
typedef struct _File   File;
typedef struct _Fpool  Fpool;

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
    char *   name;
    char *   data;
    uint16_t permissions;
    char     c_date[TIME_BUF_SIZE];        //creation date
    char     m_date[TIME_BUF_SIZE];        //modification date
    uint32_t size;
    uint32_t w_point;      //write point
    uint32_t r_point;      //read point
    bool     is_opened;
    Fpool   *fpool;
};

struct _Folder
{
    char *   name;
    char     c_date[TIME_BUF_SIZE];    //creation date
    uint16_t permissions;
    Fpool   *fpool;
};

// Defines
#define READ_ONLY                  1
#define WRITE_ONLY                 2
#define EXEC_ONLY                  4

#define SUCCESS                    0
#define FAIL                      -1
#define FILE_NOT_FOUND            -2
#define FILE_ALREADY_EXIST        -3
#define FILE_IS_ALREADY_OPENED    -4
#define FILE_IS_ALREADY_CLOSED    -5
#define THERE_ARE_NOT_FILES       -6

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__DEFINES_H__)
