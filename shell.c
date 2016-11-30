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

static void printCdHelp(bool showFlagsDetails);
static void printLsHelp(bool showFlagsDetails);
static void printLlHelp(bool showFlagsDetails);
static void printDirHelp(bool showFlagsDetails);
static void printRmHelp(bool showFlagsDetails);
static void printMkdirHelp(bool showFlagsDetails);
static void printPwdHelp(bool showFlagsDetails);
static void printCpHelp(bool showFlagsDetails);
static void printMvHelp(bool showFlagsDetails);
static void printTouchHelp(bool showFlagsDetails);
static void printChmodHelp(bool showFlagsDetails);
static void printSudoSuHelp(bool showFlagsDetails);
static void printEchoHelp(bool showFlagsDetails);
static void printEditHelp(bool showFlagsDetails);
static void printCleanHelp(bool showFlagsDetails);
static void printExitHelp(bool showFlagsDetails);

static int32_t runCommand(char **args, uint32_t nargs);
static int32_t runHelp(char **args, uint32_t nargs);
static int32_t runCd(char **args, uint32_t nargs);
static int32_t runLs(char **args, uint32_t nargs);
static int32_t runLl(char **args, uint32_t nargs);
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
static int32_t runExit(char **args, uint32_t nargs);
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
        case COMMAND_NOT_FOUND:       printf("\nERROR: Command not found\n\n");
            break;
        case INVALID_PARAMETERS:      printf("\nERROR: Invalid Parameters\n\n");
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
        //TODO: implement this command as function
        if (strcmp("exit", args[0]) == 0)
        {
            //exit of shell
            ret = false;
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
        else if (strcmp("ll", args[0]) == 0)
        {
            ret = runLl(args, nargs);
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
        else if (strcmp("exit", args[0]) == 0)
        {
            ret = runExit(args, nargs);
        }
        else
        {
            ret = COMMAND_NOT_FOUND;
        }
    }

    return ret;
}

static void printCdHelp(bool showFlagsDetails)
{
    printf("\ncd      : change directory\n");
    printf("\nSintax:\n\ncd <flags[optionals]> <path>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
    }
}

static void printLsHelp(bool showFlagsDetails)
{
    printf("\nls      : list files\n");
    printf("\nSintax:\n\nls <flags[optionals]> <path[optional]>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
        printf("-i    : Show file details\n");
    }
}

static void printLlHelp(bool showFlagsDetails)
{
    printf("\nll      : list files with details\n");
    printf("\nSintax:\n\nll <flags[optionals]> <path[optional]>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
    }
}

static void printDirHelp(bool showFlagsDetails)
{
    printf("\ndir     : show directory\n");
    printf("\nSintax:\n\ndir <flags[optionals]> <path[optional]>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
        printf("-i    : Show file details\n");
    }
}

static void printRmHelp(bool showFlagsDetails)
{
    printf("\nrm      : remove file\n");
    printf("\nSintax:\n\nrm <flags[optionals]> <path>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
        printf("-f    : Force delete\n");
        printf("-r    : Recursive\n");
    }
}

static void printMkdirHelp(bool showFlagsDetails)
{
    printf("\nmkdir   : create a directory\n");
    printf("\nSintax:\n\nmkdir <flags[optionals]> <path>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
    }
}

static void printPwdHelp(bool showFlagsDetails)
{
    printf("\npwd     : show current path\n");
    printf("\nSintax:\n\npwd <flags[optionals]>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
    }
}

static void printCpHelp(bool showFlagsDetails)
{
    printf("\ncp      : copy file\n");
    printf("\nSintax:\n\ncp <flags[optionals]> <source path> <destination path>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
        printf("-f    : Force copy (override if the file exist)\n");
    }
}

static void printMvHelp(bool showFlagsDetails)
{
    printf("\nmv      : move file\n");
    printf("\nSintax:\n\nmv <flags[optionals]> <source path> <destination path>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
        printf("-f    : Force move (override if the file exist)\n");
    }
}

static void printTouchHelp(bool showFlagsDetails)
{
    printf("\ntouch   : touch file\n");
    printf("\nSintax:\n\ntouch <flags[optionals]> <path>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
    }
}

static void printChmodHelp(bool showFlagsDetails)
{
    printf("\nchmod   : change permissions\n");
    printf("\nSintax:\n\nchmod <flags[optionals]> <new mode> <path>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
    }
}

static void printSudoSuHelp(bool showFlagsDetails)
{
    printf("\nsudo su : change to root\n");
    printf("\nSintax:\n\nsudo su <flags[optionals]>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
    }
}

static void printEchoHelp(bool showFlagsDetails)
{
    printf("\necho    : show info in screen\n");
    printf("\nSintax:\n\necho <flags[optionals]> <string>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
    }
}

static void printEditHelp(bool showFlagsDetails)
{
    printf("\nedit    : edit file\n");
    printf("\nSintax:\n\nedit <flags[optionals]> <path>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
    }
}

static void printCleanHelp(bool showFlagsDetails)
{
    printf("\nhelp    : show help\n");
    printf("\nSintax:\n\nclean <flags[optionals]>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
    }
}

static void printExitHelp(bool showFlagsDetails)
{
    printf("\nexit    : exit of shell or exit of root mode\n");
    printf("\nSintax:\n\nexit <flags[optionals]>\n\n");

    if (showFlagsDetails)
    {
        printf("Flags:\n\n");
        printf("-?    : Show help\n");
    }
}


static int32_t runHelp(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    printf("\nCommands suported:\n\n");
    printf("help    : show help\n");
    printf("cd      : change directory\n");
    printf("ls      : list files\n");
    printf("ll      : list files with details\n");
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
    printf("exit    : exit of shell or exit of root mode\n");

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
        getLastNameFromPath(args[1], &pName);

        if (pFolder != NULL)
        {
            ret = searchFileOrFolderIntoPool(pFolder, pName, NULL, &pFolder, true);

            if (ret == SUCCESS
                && pFolder != NULL)
            {
                setCurrentDirectory(pFolder); 
            }
            //if the destination folder is the root folder  "/"
            else if (pName == NULL
                     && pFolder == getRootFolder())
            {
                setCurrentDirectory(pFolder);
                ret = SUCCESS;
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
        Folder *parentFolder = NULL;
        Folder *pFolder = NULL;
        File   *pFile   = NULL;
        bool showDetails = false;

        //TODO: search the flags
        showDetails = false;

        if (nargs == 1)
        {
            parentFolder = getCurrentFolder();
            ret = printInfoOfPool(parentFolder, showDetails);
        }
        else
        {
            char   * pName = NULL;

            parentFolder = getFolderFromPath(args[1]);
            getLastNameFromPath(args[1], &pName);

            if (parentFolder != NULL)
            {
                //search for a folder
                ret = searchFileOrFolderIntoPool(parentFolder, pName, NULL, &pFolder, true);

                if (ret == SUCCESS
                    && pFolder != NULL)
                {
                    ret = printInfoOfPool(pFolder, showDetails);
                }
                else
                {
                    //search for a file
                    ret = searchFileOrFolderIntoPool(parentFolder, pName, &pFile, NULL, false);

                    if (ret == SUCCESS
                        && pFile != NULL)
                    {
                        printFileInfo(pFile, showDetails);
                    }
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

static int32_t runLl(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {
        Folder *parentFolder = NULL;
        Folder *pFolder = NULL;
        File   *pFile   = NULL;

        if (nargs == 1)
        {
            parentFolder = getCurrentFolder();
            ret = printInfoOfPool(parentFolder, true);
        }
        else
        {
            char   * pName = NULL;

            parentFolder = getFolderFromPath(args[1]);
            getLastNameFromPath(args[1], &pName);

            if (parentFolder != NULL)
            {
                //search for a folder
                ret = searchFileOrFolderIntoPool(parentFolder, pName, NULL, &pFolder, true);

                if (ret == SUCCESS
                    && pFolder != NULL)
                {
                    ret = printInfoOfPool(pFolder, true);
                }
                else
                {
                    //search for a file
                    ret = searchFileOrFolderIntoPool(parentFolder, pName, &pFile, NULL, false);

                    if (ret == SUCCESS
                        && pFile != NULL)
                    {
                        printFileInfo(pFile, true);
                    }
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
        runLs(args, nargs);
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
        getLastNameFromPath(args[1], &pName);

        if (pFolder != NULL)
        {
            ret = searchFileOrFolderIntoPool(pFolder, pName, &pFile, NULL, false);

            if (ret == SUCCESS
                && pFile != NULL)
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
            getLastNameFromPath(args[i], &pName);

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
            printf("%s\n", pFullPath);
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
        File   * pFile = NULL;
        char   * pName = NULL;

        for (i = 1; i < nargs; i++)
        {
            parentFolder = getFolderFromPath(args[i]);
            getLastNameFromPath(args[i], &pName);

            ret = searchFileOrFolderIntoPool(parentFolder, pName, &pFile, NULL, false);

            if (ret == SUCCESS
                && pFile != NULL)
            {
                ret = updateModificationDate(pFile);
            }
            else if (parentFolder != NULL) 
            {
                pFile = createNewFile(parentFolder, pName, READ_ONLY | WRITE_ONLY | EXEC_ONLY);

                if (pFile != NULL)
                {
                    ret = SUCCESS;
                    pFile = NULL;
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

static int32_t runExit(char **args, uint32_t nargs)
{
    int32_t ret = SUCCESS;

    if (args != NULL
        && nargs > 0)
    {
        
    }

    return ret;
}

