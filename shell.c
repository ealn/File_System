/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Shell implementation
 */

#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "memutils.h"
#include "console_utils.h"
#include "file_system.h"
#include "file.h"
#include "folder.h"
#include "f_pool.h"

#define PROMPT_SIZE   100
#define DIST_NAME     "File-System-1.0: "
#define SEPARATOR     "@"
#define COMMAND_CHAR  " $ "

static void createPrompt(char *pPromptStr);
static bool validateArg(char **args);
static int32_t runCommand(char **args, uint32_t nargs);
static int32_t runHelp(char **args, uint32_t nargs);
static int32_t runCd(char **args, uint32_t nargs);
static int32_t runLs(char **args, uint32_t nargs);
static int32_t runDir(char **args, uint32_t nargs);
static int32_t runRm(char **args, uint32_t nargs);
static int32_t runMkdir(char **args, uint32_t nargs);
static int32_t runPwd(char **args, uint32_t nargs);
static int32_t runCp(char **args, uint32_t nargs);
static int32_t runMv(char **args, uint32_t nargs);
static int32_t runTouch(char **args, uint32_t nargs);
static int32_t runChmod(char **args, uint32_t nargs);
static int32_t runSudo(char **args, uint32_t nargs);
static int32_t runEcho(char **args, uint32_t nargs);
static int32_t runEdit(char **args, uint32_t nargs);
static int32_t runClean(char **args, uint32_t nargs);
static int32_t runCat(char **args, uint32_t nargs);
static void printOutput(int32_t ret);

int32_t runShell(void)
{
    int32_t ret = SUCCESS;
    bool loop = true;
    char **args = NULL;
    uint32_t nargs = 0;
    char promptStr[PROMPT_SIZE];

    cleanScreen();

    do
    {
        createPrompt(promptStr);
        getArgumentsFromConsole(promptStr, &args, &nargs);

        if (args != NULL
            && nargs > 0)
        {
            if (validateArg(args))
            {
                ret = runCommand(args, nargs);
                printOutput(ret);
            }
            else
            {
                loop = false;
            }

            destroyStringsParsed(args, nargs);
        }
    }while (loop); 

    return ret;
}

static void createPrompt(char *pPromptStr)
{
    char *userName = NULL;
    char *currentFolder = NULL;
    uint32_t len = 0;

    memset(pPromptStr, 0, sizeof(char) * PROMPT_SIZE);

    userName = getCurrentUser();
    currentFolder = getCurrentFolderName();

    if (userName != NULL)
    {
        strcpy(pPromptStr, userName);
        len = strlen(userName);
    }

    strcpy(pPromptStr + len, SEPARATOR);
    len += strlen(SEPARATOR);

    strcpy(pPromptStr + len, DIST_NAME);
    len += strlen(DIST_NAME);

    if (currentFolder != NULL)
    {
        strcpy(pPromptStr + len, currentFolder);
        len += strlen(currentFolder);
    }

    strcpy(pPromptStr + len, COMMAND_CHAR);
}

static void printOutput(int32_t ret)
{
    switch (ret)
    {
        case FAIL:                    printf("\nERROR: Something wrong happened with the command\n\n");
            break;
        case FILE_NOT_FOUND:          printf("\nERROR: File not found\n\n");
            break;
        case FILE_ALREADY_EXIST:      printf("\nERROR: File already exist\n\n");
            break;
        case FILE_IS_ALREADY_OPENED:  printf("\nERROR: File already opened\n\n");
            break;
        case FILE_IS_ALREADY_CLOSED:  printf("\nERROR: File already closed\n\n");
            break;
        case THERE_ARE_NOT_FILES:     printf("\nERROR: There are not files\n\n");
            break;
        case FOLDER_NOT_FOUND:        printf("\nERROR: Folder not found\n\n");
            break;
        case FOLDER_ALREADY_EXIST:    printf("\nERROR: Folder already exist\n\n");
            break;
        default:
            break;
    }
}

static bool validateArg(char **args)
{
    bool ret = true;

    if (args != NULL)
    {
        if (strcmp("exit", args[0]) == 0)
        {
            //exit of shell
            ret = false;
        }
    }

    return ret;
}

static int32_t getName(char *path, char **output)
{
    int32_t ret = SUCCESS;

    if (path != NULL
        && output != NULL)
    {
        char    **listOut = NULL;
        uint32_t  numberOfElements = 0;
        uint32_t  length = 0;

        parseString(path, FOLDER_SEPARATOR, &listOut, &numberOfElements);

        if (listOut != NULL)
        {
            length = strlen(listOut[numberOfElements - 1]) + 1;

            *output = (char *)MEMALLOC(sizeof(char) * length);
            memcpy(*output, listOut[numberOfElements - 1], sizeof(char) * (length - 1));
            destroyStringsParsed(listOut, numberOfElements);
        }
    }

    return ret; 
}

static int32_t runCommand(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL)
    {
        //list of commands
        if (strcmp("help", args[0]) == 0)
        {
            ret = runHelp(args, nargs);
        }
        else if (strcmp("cd", args[0]) == 0)
        {
            ret = runCd(args, nargs);
        }
        else if (strcmp("ls", args[0]) == 0)
        {
            ret = runLs(args, nargs);
        }
        else if (strcmp("dir", args[0]) == 0)
        {
            ret = runDir(args, nargs);
        }
        else if (strcmp("rm", args[0]) == 0) 
        {
            ret = runRm(args, nargs);
        }
        else if (strcmp("mkdir", args[0]) == 0)
        {
            ret = runMkdir(args, nargs);
        }
        else if (strcmp("pwd", args[0]) == 0)
        {
            ret = runPwd(args, nargs);
        }
        else if (strcmp("cp", args[0]) == 0)
        {
            ret = runCp(args, nargs);
        }
        else if (strcmp("mv", args[0]) == 0)
        {
            ret = runMv(args, nargs);
        }
        else if (strcmp("touch", args[0]) == 0)
        {
            ret = runTouch(args, nargs);
        }
        else if (strcmp("chmod", args[0]) == 0)
        {
            ret = runChmod(args, nargs);
        }
        else if (strcmp("sudo", args[0]) == 0)
        {
            ret = runSudo(args, nargs);
        }
        else if (strcmp("echo", args[0]) == 0)
        {
            ret = runEcho(args, nargs);
        }
        else if (strcmp("edit", args[0]) == 0)
        {
            ret = runEdit(args, nargs);
        }
        else if (strcmp("clean", args[0]) == 0)
        {
            ret = runClean(args, nargs);
        }
        else if (strcmp("cat", args[0]) == 0)
        {
            ret = runCat(args, nargs);
        }
        else
        {
            ret = FAIL;
            printf("\nCommand not suported\n");
        }
    }

    return ret;
}

static int32_t runHelp(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    printf("\nCommand suported:\n\n");
    printf("exit    : exit of shell\n");
    printf("help    : show help\n");
    printf("cd      : change directory\n");
    printf("ls      : list files\n");
    printf("dir     : show directory\n");
    printf("rm      : remove file\n");
    printf("mkdir   : create a directory\n");
    printf("pwd     : show current path\n");
    printf("cp      : copy file\n");
    printf("mv      : move file\n");
    printf("touch   : touch file\n");
    printf("chmod   : change permissions\n");
    printf("sudo su : change to root\n");
    printf("echo    : show info in screen\n");
    printf("edit    : edit file\n");
    printf("clean   : clean screen\n\n");

    return ret;
}

static int32_t runCd(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 1)
    {
        Folder * pFolder = NULL;
        char   * pName = NULL;

        pFolder = getFolderFromPath(args[1]);
        getName(args[1], &pName);

        if (pFolder != NULL)
        {
            ret = searchFileOrFolderIntoPool(pFolder, pName, NULL, &pFolder, true);

            if (pFolder != NULL)
            {
                setCurrentDirectory(pFolder); 
            }
        }

        if (pName != NULL) 
        {
            MEMFREE(pName); 
        }
    }

    return ret; 
}

static int32_t runLs(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {
        Folder *pFolder = NULL;

        if (nargs == 1)
        {
            pFolder = getCurrentFolder();
            ret = printInfoOfPool(pFolder);
        }
        else
        {
            char   * pName = NULL;

            pFolder = getFolderFromPath(args[1]);
            getName(args[1], &pName);

            if (pFolder != NULL)
            {
                ret = searchFileOrFolderIntoPool(pFolder, pName, NULL, &pFolder, true);

                if (pFolder != NULL)
                {
                    ret = printInfoOfPool(pFolder);
                }
            }

            if (pName != NULL) 
            {
                MEMFREE(pName); 
            }
        }
    }

    return ret;
}

static int32_t runDir(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {
        Folder *pFolder = NULL;

        if (nargs == 1)
        {
            pFolder = getCurrentFolder();
            ret = printInfoOfPool(pFolder);
        }
        else
        {
            char   * pName = NULL;

            pFolder = getFolderFromPath(args[1]);
            getName(args[1], &pName);

            if (pFolder != NULL)
            {
                ret = searchFileOrFolderIntoPool(pFolder, pName, NULL, &pFolder, true);

                if (pFolder != NULL)
                {
                    ret = printInfoOfPool(pFolder);
                }
            }

            if (pName != NULL) 
            {
                MEMFREE(pName); 
            }
        }
    }

    return ret;
}

static int32_t runRm(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {
        Folder * pFolder = NULL;
        File   * pFile = NULL;
        char   * pName = NULL;

        pFolder = getFolderFromPath(args[1]);
        getName(args[1], &pName);

        if (pFolder != NULL)
        {
            ret = searchFileOrFolderIntoPool(pFolder, pName, &pFile, NULL, false);

            if (pFile != NULL)
            {
                ret = destroyFile(pFile);
            }
        }

        if (pName != NULL) 
        {
            MEMFREE(pName); 
        }
    }

    return ret;
}

static int32_t runMkdir(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 1)
    {
        uint32_t i = 0;
        Folder * parentFolder = NULL;
        char  *  pName = NULL;

        for (i = 1; i < nargs; i++)
        {
            parentFolder = getFolderFromPath(args[i]);
            getName(args[i], &pName);

            if (parentFolder != NULL)
            {
                createNewFolder(parentFolder, pName, NULL);
            }
            if (pName != NULL) 
            {
                MEMFREE(pName); 
            }
        }
    }

    return ret;
}

static int32_t runPwd(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {
        Folder *currentFolder = NULL;
        char * pFullPath = NULL;

        currentFolder = getCurrentFolder();
        pFullPath = getFullPath(currentFolder);

        if (pFullPath != NULL)
        {
            printf("%s", pFullPath);
            MEMFREE(pFullPath);
        }
        else
        {
            ret = FAIL;
        }
    }

    return ret;
}

static int32_t runCp(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {

    }

    return ret;
}

static int32_t runMv(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {

    }

    return ret;
}

static int32_t runTouch(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {
        uint32_t i = 0;
        Folder * parentFolder = NULL;
        char  *  pName = NULL;

        for (i = 1; i < nargs; i++)
        {
            parentFolder = getFolderFromPath(args[i]);
            getName(args[i], &pName);

            //TODO: check if the file exist then the modification date needs to be changed
            if (parentFolder != NULL)
            {
                createNewFile(parentFolder, pName, READ_ONLY | WRITE_ONLY | EXEC_ONLY);
            }
            if (pName != NULL) 
            {
                MEMFREE(pName); 
            }
        }
    }

    return ret;
}

static int32_t runChmod(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {

    }

    return ret;
}

static int32_t runSudo(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {

    }

    return ret;
}

static int32_t runEcho(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {
        uint32_t i = 0;

        for (i = 1; i < nargs; i++)
        {
            printf("%s ", args[i]); 
        }

        printf("\n");
    }

    return ret;
}

static int32_t runEdit(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {

    }

    return ret;
}

static int32_t runClean(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {
        cleanScreen();
    }

    return ret;
}

static int32_t runCat(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {
        
    }

    return ret;
}

