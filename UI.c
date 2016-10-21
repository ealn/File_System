/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Implementation of User interface
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "UI.h"
#include "file.h"
#include "trace.h"
#include "console_utils.h"
#include "resources.h"

//Constanst
#define CREATE_FILE           1
#define OPEN_FILE             2
#define CLOSE_FILE            3
#define DELETE_FILE           4
#define WRITE_FILE            5
#define READ_FILE             6
#define TELL_FILE             7
#define SEEK_FILE             8
#define EXIT                  9

#define WRITE_BUF_SIZE        2048
#define READ_BUF_SIZE         2048

//static functions
static void printOutput(int32_t ret);
static int32_t showMainMenu(uint8_t *optionSelected);
static int32_t showCreateFileMenu(void);
static int32_t showOpenFileMenu(void);
static int32_t showCloseFileMenu(void);
static int32_t showDeleteFileMenu(void);
static int32_t showWriteFileMenu(void);
static int32_t showReadFileMenu(void);
static int32_t showTellFileMenu(void);
static int32_t showSeekFileMenu(void);

int32_t showUI(void)
{
    int32_t ret = SUCCESS;
    uint8_t optionSelected = 0;
    bool repeat = false;

    do
    {
        repeat = false;
        
        ret = showMainMenu(&optionSelected);
    
        UI_DEBUG("optionSelected=%i\n", optionSelected);

        if (optionSelected != EXIT)
        {
            cleanScreen();
        }

        switch (optionSelected) 
        {
            case CREATE_FILE: ret = showCreateFileMenu();
                break;
            case OPEN_FILE: ret = showOpenFileMenu();
                break;
            case CLOSE_FILE: ret = showCloseFileMenu();
                break;
            case DELETE_FILE: ret = showDeleteFileMenu();
                break;
            case WRITE_FILE: ret = showWriteFileMenu();
                break;
            case READ_FILE: ret = showReadFileMenu();
                break;
            case TELL_FILE: ret = showTellFileMenu();
                break;
            case SEEK_FILE: ret = showSeekFileMenu();
                break;
            default: break;
        }
        
        if (optionSelected != EXIT)
        {
            //waiting for a key input
            getchar();
            repeat = true;
        }
    }while (repeat);

    return ret;
}

static int32_t showMainMenu(uint8_t *optionSelected)
{
    int32_t ret = SUCCESS;
    bool    isValidOption = true;

    if (optionSelected != NULL)
    {
        *optionSelected = createMenuWithMultipleOptions(STR_FILE_SYSTEM_TITLE,
                                                        STR_SELECT_OPTION,
                                                        STR_MAIN_MENU_OPTIONS,
                                                        STR_OPTION_SELECTED,
                                                        true,
                                                        CREATE_FILE,
                                                        EXIT,
                                                        true);

        UI_DEBUG("optionSelected=%i\n", *optionSelected);
    }
    else
    {
        UI_ERROR("optionSelected is null");
        ret = FAIL;
    }
    
    return ret;
}

static void printOutput(int32_t ret)
{
    switch (ret)
    {
        case SUCCESS:                 printf(STR_SUCCESS);
            break;
        case FAIL:                    printf(STR_FAIL);
            break;
        case FILE_NOT_FOUND:          printf(STR_FILE_NOT_FOUND);
            break;
        case FILE_ALREADY_EXIST:      printf(STR_FILE_ALREADY_EXIST);
            break;
        case FILE_IS_ALREADY_OPENED:  printf(STR_FILE_IS_ALREADY_OPENED);
            break;
        case FILE_IS_ALREADY_CLOSED:  printf(STR_FILE_IS_ALREADY_CLOSED);
            break;
        default:
            break;
    }
}

static int32_t showCreateFileMenu(void)
{
    int32_t ret = SUCCESS;
    char fileName[FILE_NAME_MAX_SIZE];

    memset(fileName, 0 , sizeof(char)*FILE_NAME_MAX_SIZE);

    getStringFromConsole(STR_FILE_NAME, fileName, (FILE_NAME_MAX_SIZE - 1)); //keep the \0 terminator

    ret = createNewFile(fileName);

    printOutput(ret);

    return ret;
}

static int32_t showOpenFileMenu(void)
{
    int32_t ret = SUCCESS;
    char fileName[FILE_NAME_MAX_SIZE];

    memset(fileName, 0 , sizeof(char)*FILE_NAME_MAX_SIZE);

    getStringFromConsole(STR_FILE_NAME, fileName, (FILE_NAME_MAX_SIZE - 1)); //keep the \0 terminator

    ret = openFile(fileName);

    printOutput(ret);

    return ret;
}

static int32_t showCloseFileMenu(void)
{
    int32_t ret = SUCCESS;
    char fileName[FILE_NAME_MAX_SIZE];

    memset(fileName, 0 , sizeof(char)*FILE_NAME_MAX_SIZE);

    getStringFromConsole(STR_FILE_NAME, fileName, (FILE_NAME_MAX_SIZE - 1)); //keep the \0 terminator

    ret = closeFile(fileName);

    printOutput(ret);

    return ret;
}

static int32_t showDeleteFileMenu(void)
{
    int32_t ret = SUCCESS;
    char fileName[FILE_NAME_MAX_SIZE];

    memset(fileName, 0 , sizeof(char)*FILE_NAME_MAX_SIZE);

    getStringFromConsole(STR_FILE_NAME, fileName, (FILE_NAME_MAX_SIZE - 1)); //keep the \0 terminator

    ret = deleteFile(fileName);

    printOutput(ret);

    return ret;
}

static int32_t showWriteFileMenu(void)
{
    int32_t ret = SUCCESS;
    char fileName[FILE_NAME_MAX_SIZE];
    char writeBuf[WRITE_BUF_SIZE];
    uint32_t len = 0;

    memset(fileName, 0 , sizeof(char)*FILE_NAME_MAX_SIZE);
    memset(writeBuf, 0 , sizeof(char)*WRITE_BUF_SIZE);

    getStringFromConsole(STR_FILE_NAME, fileName, (FILE_NAME_MAX_SIZE - 1)); //keep the \0 terminator
    getStringFromConsole(STR_WRITE_FILE, writeBuf, (WRITE_BUF_SIZE - 1)); //keep the \0 terminator

    len = strlen(writeBuf);

    ret = writeFile(fileName, writeBuf, len);

    printOutput(ret);

    return ret;
}

static int32_t showReadFileMenu(void)
{
    int32_t   ret = SUCCESS;
    char      fileName[FILE_NAME_MAX_SIZE];
    char      readBuf[READ_BUF_SIZE];
    uint32_t  numberOfBytes = 0;

    memset(fileName, 0 , sizeof(char)*FILE_NAME_MAX_SIZE);
    memset(readBuf, 0 , sizeof(char)*READ_BUF_SIZE);

    getStringFromConsole(STR_FILE_NAME, fileName, (FILE_NAME_MAX_SIZE - 1)); //keep the \0 terminator
    numberOfBytes = getUInt32FromConsole(STR_READ_FILE);

    if (numberOfBytes > READ_BUF_SIZE)
    {
        numberOfBytes = READ_BUF_SIZE;
    }

    ret = readFile(fileName, readBuf, numberOfBytes); 

    if (ret == SUCCESS)
    {
        printf("\n%s",readBuf); 
    }

    printOutput(ret);

    return ret;
}

static int32_t showTellFileMenu(void)
{
    int32_t ret = SUCCESS;
    char fileName[FILE_NAME_MAX_SIZE];
    uint32_t position = 0;

    memset(fileName, 0 , sizeof(char)*FILE_NAME_MAX_SIZE);

    getStringFromConsole(STR_FILE_NAME, fileName, (FILE_NAME_MAX_SIZE - 1)); //keep the \0 terminator

    ret = tellFile(fileName, &position);

    if (ret == SUCCESS)
    {
        printf(STR_TELL_FILE, position);
    }

    printOutput(ret);

    return ret;
}

static int32_t showSeekFileMenu(void)
{
    int32_t ret = SUCCESS;
    char fileName[FILE_NAME_MAX_SIZE];
    uint32_t position = 0;

    memset(fileName, 0 , sizeof(char)*FILE_NAME_MAX_SIZE);

    getStringFromConsole(STR_FILE_NAME, fileName, (FILE_NAME_MAX_SIZE - 1)); //keep the \0 terminator
    position = getUInt32FromConsole(STR_SEEK_FILE);

    ret = seekFile(fileName, position);

    printOutput(ret);

    return ret;
}
