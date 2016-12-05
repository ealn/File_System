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

#define PROMPT_SIZE      100
#define DIST_NAME        "File-System-1.0: "
#define SEPARATOR        "@"
#define COMMAND_CHAR     " $ "
#define SEND_TO_FILE     '>'
#define APPEND_TO_FILE   ">>"
#define FLAG_INDICATOR   '-'

//Flags
#define HELP_FLAG              "-?"
#define LS_SHOW_DETAILS_FLAG   "-l"
#define RM_FORCE_FLAG          "-f"
#define RM_RECURSIVE_FLAG      "-r"
#define CP_FORCE_FLAG          "-f"
#define MV_FORCE_FLAG          "-f"

//Structs
typedef struct _Arguments
{
    char     *arg;
    uint32_t  index;
}Arguments;

typedef struct _ParamList
{
    Arguments  * flags;
    Arguments  * parameters;
    uint32_t     numberOfFlags;
    uint32_t     numberOfParameters;
}ParamList;

static void createPrompt(char *pPromptStr);
static bool validateArg(char **args);
static void printOutput(int32_t ret);

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
static int32_t runHelp(ParamList *pParamList);
static int32_t runCd(ParamList *pParamList);
static int32_t runLs(ParamList *pParamList);
static int32_t runLl(ParamList *pParamList);
static int32_t runDir(ParamList *pParamList);
static int32_t runRm(ParamList *pParamList);
static int32_t runMkdir(ParamList *pParamList);
static int32_t runPwd(ParamList *pParamList);
static int32_t runCp(ParamList *pParamList);
static int32_t runMv(ParamList *pParamList);
static int32_t runTouch(ParamList *pParamList);
static int32_t runChmod(ParamList *pParamList);
static int32_t runSudo(ParamList *pParamList);
static int32_t runEcho(ParamList *pParamList);
static int32_t runEdit(ParamList *pParamList);
static int32_t runClean(ParamList *pParamList);
static int32_t runCat(ParamList *pParamList);
static int32_t runExit(ParamList *pParamList);

static Arguments * allocArguments(uint32_t numberOfArgs)
{
    Arguments * pArguments = NULL;
    
    if (numberOfArgs > 0)
    {
        pArguments = (Arguments *)MEMALLOC(sizeof(Arguments)*numberOfArgs); 
    }
    
    return pArguments;
}

static void freeArguments(Arguments *pArguments)
{
    if (pArguments != NULL)
    {
        MEMFREE((void *)pArguments);
    }
}

static ParamList * allocParamList(void)
{
    ParamList * pParamList = NULL;
    
    pParamList = (ParamList *)MEMALLOC(sizeof(ParamList));
    
    return pParamList;
}

static void freeParamList(ParamList *pParamList)
{
    if (pParamList != NULL)
    {
        MEMFREE((void *)pParamList);
    }
}

static ParamList * createParamList(uint32_t numberOfFlags, uint32_t numberOfParameters)
{
    ParamList * pParamList = NULL;

    pParamList = allocParamList();

    if (pParamList != NULL)
    {
        pParamList->flags = allocArguments(numberOfFlags);
        pParamList->parameters = allocArguments(numberOfParameters);
        pParamList->numberOfFlags = numberOfFlags;
        pParamList->numberOfParameters = numberOfParameters;
    }

    return pParamList; 
}

static void destroyParamList(ParamList * pParamList)
{
    if (pParamList != NULL)
    {
        if (pParamList->flags != NULL)
        {
            freeArguments(pParamList->flags);
        }
        if (pParamList->parameters != NULL)
        {
            freeArguments(pParamList->flags);
        }

        freeParamList(pParamList);
    }
}

static Arguments * getArgumentAtIndex(Arguments * pArguments, uint32_t index)
{
    Arguments * pArg = NULL;

    if (pArguments != NULL)
    {
        pArg = pArguments + index;
    }

    return pArg; 
}

static ParamList * processParameters(char **args, uint32_t nargs)
{
    ParamList * pParamList = NULL;

    if (args != NULL
        && nargs > 0)
    {
        Arguments * pArg = NULL;
        char      * argument = NULL;
        uint32_t    i = 0;
        uint32_t    indexFlags = 0;
        uint32_t    indexParams = 0;
        uint32_t    numberOfFlags = 0;
        uint32_t    numberOfParameters = 0;

        //calculate the number of flags
        if (nargs > 1)
        {
            for (i = 1; i < nargs; i++) 
            {
                argument = args[i];

                if (argument[0] == FLAG_INDICATOR
                    || argument[0] == SEND_TO_FILE)
                {
                    numberOfFlags++;
                }
                else
                {
                    numberOfParameters++;
                }
            }
        }

        //create new parameter list
        pParamList = createParamList(numberOfFlags, numberOfParameters);

        //Add arguments to the list of parameters
        if (nargs > 1)
        {
            for (i = 1; i < nargs; i++) 
            {
                argument = args[i];

                if (argument[0] == FLAG_INDICATOR
                    || argument[0] == SEND_TO_FILE)
                {
                    pArg = getArgumentAtIndex(pParamList->flags, indexFlags);
                    indexFlags++;
                }
                else
                {
                    pArg = getArgumentAtIndex(pParamList->parameters, indexParams);
                    indexParams++;
                }

                pArg->arg = argument;
                pArg->index = i;
            }
        }
    }

    return pParamList;
}

static bool searchFlag(ParamList * pParamList, const char *flag)
{
    bool ret = false;

    if (pParamList != NULL
        && pParamList->flags != NULL
        && pParamList->numberOfFlags > 0)
    {
        Arguments *pArg = NULL;
        uint32_t   i = 0;

        for (i = 0; i < pParamList->numberOfFlags; i++)
        {
            pArg = getArgumentAtIndex(pParamList->flags, i);

            if (pArg != NULL
                && strcmp(pArg->arg, flag) == 0)
            {
                ret = true;
            }
        }
    }

    return ret; 
}

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
        case FAIL:                        printf("\nERROR: Something wrong happened with the command\n\n");
            break;                      
        case FILE_NOT_FOUND:              printf("\nERROR: File not found\n\n");
            break;                      
        case FILE_ALREADY_EXIST:          printf("\nERROR: File already exist\n\n");
            break;                      
        case FILE_IS_ALREADY_OPENED:      printf("\nERROR: File already opened\n\n");
            break;                      
        case FILE_IS_ALREADY_CLOSED:      printf("\nERROR: File already closed\n\n");
            break;                      
        case THERE_ARE_NOT_FILES:         printf("\nERROR: There are not files\n\n");
            break;                      
        case FOLDER_NOT_FOUND:            printf("\nERROR: Folder not found\n\n");
            break;                      
        case FOLDER_ALREADY_EXIST:        printf("\nERROR: Folder already exist\n\n");
            break;                      
        case COMMAND_NOT_FOUND:           printf("\nERROR: Command not found\n\n");
            break;                      
        case INVALID_PARAMETERS:          printf("\nERROR: Invalid Parameters\nFor more information type the command followed by the flag -?\n\n");
            break;                      
        case FILE_CAN_NOT_BE_DELETED:     printf("\nERROR: File can not be deleted\n\n");
            break;
        case FOLDER_CAN_NOT_BE_DELETED:   printf("\nERROR: Folder can not be deleted\n\n");
            break;
        case FILE_CAN_NOT_BE_OVERWRITTEN: printf("\nERROR: File can not be overwritten\n\n");
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
    int32_t     ret = SUCCESS;
    ParamList * pParamList = NULL;

    if (args != NULL)
    {
        pParamList = processParameters(args, nargs);

        //list of commands
        if (strcmp("help", args[0]) == 0)
        {
            ret = runHelp(pParamList);
        }
        else if (strcmp("cd", args[0]) == 0)
        {
            ret = runCd(pParamList);
        }
        else if (strcmp("ls", args[0]) == 0)
        {
            ret = runLs(pParamList);
        }
        else if (strcmp("ll", args[0]) == 0)
        {
            ret = runLl(pParamList);
        }
        else if (strcmp("dir", args[0]) == 0)
        {
            ret = runDir(pParamList);
        }
        else if (strcmp("rm", args[0]) == 0) 
        {
            ret = runRm(pParamList);
        }
        else if (strcmp("mkdir", args[0]) == 0)
        {
            ret = runMkdir(pParamList);
        }
        else if (strcmp("pwd", args[0]) == 0)
        {
            ret = runPwd(pParamList);
        }
        else if (strcmp("cp", args[0]) == 0)
        {
            ret = runCp(pParamList);
        }
        else if (strcmp("mv", args[0]) == 0)
        {
            ret = runMv(pParamList);
        }
        else if (strcmp("touch", args[0]) == 0)
        {
            ret = runTouch(pParamList);
        }
        else if (strcmp("chmod", args[0]) == 0)
        {
            ret = runChmod(pParamList);
        }
        else if (strcmp("sudo", args[0]) == 0)
        {
            ret = runSudo(pParamList);
        }
        else if (strcmp("echo", args[0]) == 0)
        {
            ret = runEcho(pParamList);
        }
        else if (strcmp("edit", args[0]) == 0)
        {
            ret = runEdit(pParamList);
        }
        else if (strcmp("clean", args[0]) == 0)
        {
            ret = runClean(pParamList);
        }
        else if (strcmp("cat", args[0]) == 0)
        {
            ret = runCat(pParamList);
        }
        else if (strcmp("exit", args[0]) == 0)
        {
            ret = runExit(pParamList);
        }
        else
        {
            ret = COMMAND_NOT_FOUND;
        }

        destroyParamList(pParamList);
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


static int32_t runHelp(ParamList *pParamList)
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
    printf("clean   : clean screen\n");
    printf("exit    : exit of shell or exit of root mode\n\n");
    printf("For more information type the command followed by the flag -?\n");
    printf("Example:\n\ncp -?\n\n");

    return ret;
}

static int32_t runCd(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printCdHelp(true);
        }
        else
        {
            if (pParamList->numberOfParameters == 0)
            {
                ret = INVALID_PARAMETERS;
            }
            else if (pParamList->numberOfParameters == 1)
            {
                Folder    * pFolder = NULL; 
                char      * pName = NULL;
                Arguments * pArg = NULL;
                
                pArg = getArgumentAtIndex(pParamList->parameters, 0);
                
                if (pArg != NULL)
                {
                    pFolder = getFolderFromPath(pArg->arg); 
                    getLastNameFromPath(pArg->arg, &pName);
                
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
            }
            else
            {
                ret = INVALID_PARAMETERS;
            }
        }
    }

    return ret; 
}

static int32_t runLs(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printLsHelp(true);
        }
        else
        {
            Folder *parentFolder = NULL; 
            Folder *pFolder = NULL;
            File   *pFile   = NULL;
            char   *pName = NULL;
            bool    showDetails = false;

            showDetails = searchFlag(pParamList, LS_SHOW_DETAILS_FLAG);

            if (pParamList->numberOfParameters == 0)
            {
                parentFolder = getCurrentFolder();
                ret = printInfoOfPool(parentFolder, showDetails);
            }
            else if (pParamList->numberOfParameters == 1) 
            {
                Arguments * pArg = NULL;
                
                pArg = getArgumentAtIndex(pParamList->parameters, 0);

                if (pArg != NULL)
                {
                    parentFolder = getFolderFromPath(pArg->arg); 
                    getLastNameFromPath(pArg->arg, &pName);

                    if (parentFolder != NULL)
                    {
                        //search for a folder
                        ret = searchFileOrFolderIntoPool(parentFolder, pName, NULL, &pFolder, true);

                        if (ret == SUCCESS
                            && pFolder != NULL)
                        {
                            ret = printInfoOfPool(pFolder, showDetails);
                        }
                        else //if the folder was not found
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
            else
            {
                ret = INVALID_PARAMETERS;
            }
        }
    }

    return ret;
}

static int32_t runLl(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printLlHelp(true);
        }
        else
        {
            Folder *parentFolder = NULL; 
            Folder *pFolder = NULL;
            File   *pFile   = NULL;
            char   *pName = NULL;
            bool    showDetails = true;

            if (pParamList->numberOfParameters == 0)
            {
                parentFolder = getCurrentFolder();
                ret = printInfoOfPool(parentFolder, showDetails);
            }
            else if (pParamList->numberOfParameters == 1) 
            {
                Arguments * pArg = NULL;
                
                pArg = getArgumentAtIndex(pParamList->parameters, 0);

                if (pArg != NULL)
                {
                    parentFolder = getFolderFromPath(pArg->arg); 
                    getLastNameFromPath(pArg->arg, &pName);

                    if (parentFolder != NULL)
                    {
                        //search for a folder
                        ret = searchFileOrFolderIntoPool(parentFolder, pName, NULL, &pFolder, true);

                        if (ret == SUCCESS
                            && pFolder != NULL)
                        {
                            ret = printInfoOfPool(pFolder, showDetails);
                        }
                        else //if the folder was not found
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
            else
            {
                ret = INVALID_PARAMETERS;
            }
        }
    }

    return ret;
}

static int32_t runDir(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printDirHelp(true);
        }
        else
        {
            Folder *parentFolder = NULL; 
            Folder *pFolder = NULL;
            File   *pFile   = NULL;
            char   *pName = NULL;
            bool    showDetails = true;

            showDetails = searchFlag(pParamList, LS_SHOW_DETAILS_FLAG);

            if (pParamList->numberOfParameters == 0)
            {
                parentFolder = getCurrentFolder();
                ret = printInfoOfPool(parentFolder, showDetails);
            }
            else if (pParamList->numberOfParameters == 1) 
            {
                Arguments * pArg = NULL;
                
                pArg = getArgumentAtIndex(pParamList->parameters, 0);

                if (pArg != NULL)
                {
                    parentFolder = getFolderFromPath(pArg->arg); 
                    getLastNameFromPath(pArg->arg, &pName);

                    if (parentFolder != NULL)
                    {
                        //search for a folder
                        ret = searchFileOrFolderIntoPool(parentFolder, pName, NULL, &pFolder, true);

                        if (ret == SUCCESS
                            && pFolder != NULL)
                        {
                            ret = printInfoOfPool(pFolder, showDetails);
                        }
                        else //if the folder was not found
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
            else
            {
                ret = INVALID_PARAMETERS;
            }
        }
    }

    return ret;
}

static int32_t runRm(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printRmHelp(true);
        }
        else
        {
            if (pParamList->numberOfParameters >= 1)
            {
                Folder    * parentFolder = NULL;
                Folder    * pFolder = NULL;
                File      * pFile = NULL;
                char      * pName = NULL;
                bool        forceDelete = false;
                bool        recursive   = false;
                uint32_t    i = 0;
                Arguments * pArg = NULL;
                
                forceDelete = searchFlag(pParamList, RM_FORCE_FLAG);
                recursive = searchFlag(pParamList, RM_RECURSIVE_FLAG);

                for (i = 0; i < pParamList->numberOfParameters; i++)
                {
                    pArg = getArgumentAtIndex(pParamList->parameters, i);

                    if (pArg != NULL)
                    {
                        parentFolder = getFolderFromPath(pArg->arg); 
                        getLastNameFromPath(pArg->arg, &pName);

                        printf("\n%s\n", pName);
                        
                        if(pName != NULL)
                        {
                            //search the file
                            ret = searchFileOrFolderIntoPool(parentFolder, pName, &pFile, NULL, false);

                            if (ret == SUCCESS
                                && pFile != NULL)
                            {
                                if (forceDelete
                                    || (strcmp(getCurrentUser(), ROOT_USER) == 0)
                                    || getYesOrNotFromConsole("Are you sure you want to delete this file [y,n]? "))
                                {
                                    ret = destroyFile(pFile); 
                                }
                                else
                                {
                                    ret = FILE_CAN_NOT_BE_DELETED;
                                }
                            }
                            else //search the folder
                            {
                                ret = searchFileOrFolderIntoPool(parentFolder, pName, NULL, &pFolder, true);

                                if (ret == SUCCESS
                                    && pFolder != NULL)
                                {
                                    if (recursive
                                        || getNumberOfChilds(pFolder) == 0)
                                    {
                                        if (forceDelete
                                            || (strcmp(getCurrentUser(), ROOT_USER) == 0)
                                            || getYesOrNotFromConsole("Are you sure you want to delete this folder [y,n]? "))
                                        {
                                            ret = destroyFolder(pFolder, recursive); 
                                        }
                                    }
                                    else
                                    {
                                        ret = FOLDER_CAN_NOT_BE_DELETED;
                                    }
                                }
                            }
                        }

                        if (pName != NULL) 
                        {
                            MEMFREE(pName); 
                        }
                    }
                }
            }
            else
            {
                ret = INVALID_PARAMETERS;
            }
        }
    }

    return ret;
}

static int32_t runMkdir(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printMkdirHelp(true);
        }
        else
        {
            if (pParamList->numberOfParameters >= 1)
            {
                uint32_t    i = 0; 
                Folder    * parentFolder = NULL;
                char      * pName = NULL;
                Arguments * pArg = NULL;

                for (i = 0; i < pParamList->numberOfParameters; i++)
                {
                    pArg = getArgumentAtIndex(pParamList->parameters, i);

                    if (pArg != NULL)
                    {
                        parentFolder = getFolderFromPath(pArg->arg); 
                        getLastNameFromPath(pArg->arg, &pName);

                        if (parentFolder != NULL)
                        {
                            createNewFolder(parentFolder, pName, NULL, DEFAULT_PERMISSIONS, NULL, NULL);
                        }
                        if (pName != NULL) 
                        {
                            MEMFREE(pName); 
                        }
                    }
                }
            }
            else
            {
                ret = INVALID_PARAMETERS;
            }
        }
    }

    return ret;
}

static int32_t runPwd(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printPwdHelp(true);
        }
        else
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
    }

    return ret;
}

static int32_t runCp(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printCpHelp(true);
        }
        else
        {
            if (pParamList->numberOfParameters == 2)
            {
                Arguments * srcArg = NULL;
                Arguments * dstArg = NULL;
                Folder    * srcFolder = NULL;
                Folder    * dstFolder = NULL;
                Folder    * parentDstFolder = NULL;
                File      * srcFile = NULL;
                File      * dstFile = NULL;
                char      * srcName = NULL;
                char      * dstName = NULL;
                bool        forceCopy = false;

                forceCopy = searchFlag(pParamList, CP_FORCE_FLAG);

                srcArg = getArgumentAtIndex(pParamList->parameters, 0);
                dstArg = getArgumentAtIndex(pParamList->parameters, 1);

                if (srcArg != NULL
                    && dstArg != NULL)
                {
                    srcFolder = getFolderFromPath(srcArg->arg); 
                    getLastNameFromPath(srcArg->arg, &srcName);

                    parentDstFolder = getFolderFromPath(dstArg->arg); 
                    getLastNameFromPath(dstArg->arg, &dstName);

                    //search the files
                    ret = searchFileOrFolderIntoPool(srcFolder, srcName, &srcFile, NULL, false);

                    if (ret == SUCCESS
                        && srcFolder != NULL
                        && srcFile != NULL
                        && parentDstFolder != NULL)
                    {
                        //search the destination folder
                        ret = searchFileOrFolderIntoPool(parentDstFolder, dstName, NULL, &dstFolder, true);

                        if (ret == SUCCESS
                            && dstFolder != NULL)
                        {
                            //search the file in the dstFolder
                            ret = searchFileOrFolderIntoPool(dstFolder, dstName, &dstFile, NULL, false);
                        }
                        else //if the folder was not found
                        {
                            //search for a file
                            ret = searchFileOrFolderIntoPool(parentDstFolder, dstName, &dstFile, NULL, false);
                        }

                        if (ret == FILE_NOT_FOUND)
                        {
                            ret = copyFiles(srcFile, dstFolder);
                        }
                        else if (dstFile != NULL) //if the file exist
                        {
                            if (srcFile == dstFile) //if the files are equals
                            {
                                ret = FILE_CAN_NOT_BE_OVERWRITTEN;
                            }
                            else
                            {
                                if (forceCopy)
                                {
                                    ret = destroyFile(dstFile);

                                    if (ret == SUCCESS)
                                    {
                                        ret = copyFiles(srcFile, dstFolder); 
                                    }
                                }
                                else
                                {
                                    ret = FILE_CAN_NOT_BE_OVERWRITTEN;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                ret = INVALID_PARAMETERS;
            }
        }
    }

    return ret;
}

static int32_t runMv(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printMvHelp(true);
        }
        else
        {
            if (pParamList->numberOfParameters == 2)
            {
                Arguments * srcArg = NULL;
                Arguments * dstArg = NULL;
                Folder    * srcFolder = NULL;
                Folder    * dstFolder = NULL;
                Folder    * parentDstFolder = NULL;
                File      * srcFile = NULL;
                File      * dstFile = NULL;
                char      * srcName = NULL;
                char      * dstName = NULL;
                bool        forceMove = false;

                forceMove = searchFlag(pParamList, MV_FORCE_FLAG);

                srcArg = getArgumentAtIndex(pParamList->parameters, 0);
                dstArg = getArgumentAtIndex(pParamList->parameters, 1);

                if (srcArg != NULL
                    && dstArg != NULL)
                {
                    srcFolder = getFolderFromPath(srcArg->arg); 
                    getLastNameFromPath(srcArg->arg, &srcName);

                    parentDstFolder = getFolderFromPath(dstArg->arg); 
                    getLastNameFromPath(dstArg->arg, &dstName);

                    //search the files
                    ret = searchFileOrFolderIntoPool(srcFolder, srcName, &srcFile, NULL, false);

                    if (ret == SUCCESS
                        && srcFolder != NULL
                        && srcFile != NULL
                        && parentDstFolder != NULL)
                    {
                        //search the destination folder
                        ret = searchFileOrFolderIntoPool(parentDstFolder, dstName, NULL, &dstFolder, true);

                        if (ret == SUCCESS
                            && dstFolder != NULL)
                        {
                            //search the file in the dstFolder
                            ret = searchFileOrFolderIntoPool(dstFolder, dstName, &dstFile, NULL, false);
                        }
                        else //if the folder was not found
                        {
                            //search for a file
                            ret = searchFileOrFolderIntoPool(parentDstFolder, dstName, &dstFile, NULL, false);
                        }

                        if (ret == FILE_NOT_FOUND)
                        {
                            ret = copyFiles(srcFile, dstFolder);

                            if (ret == SUCCESS)
                            {
                                ret = destroyFile(srcFile);
                            }
                        }
                        else if (dstFile != NULL) //if the file exist
                        {
                            if (srcFile == dstFile) //if the files are equals
                            {
                                ret = FILE_CAN_NOT_BE_OVERWRITTEN;
                            }
                            else
                            {
                                if (forceMove)
                                {
                                    ret = destroyFile(dstFile);

                                    if (ret == SUCCESS)
                                    {
                                        ret = copyFiles(srcFile, dstFolder);

                                        if (ret == SUCCESS)
                                        {
                                            ret = destroyFile(srcFile);
                                        }
                                    }
                                }
                                else
                                {
                                    ret = FILE_CAN_NOT_BE_OVERWRITTEN;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                ret = INVALID_PARAMETERS;
            }
        }
    }

    return ret;
}

static int32_t runTouch(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printTouchHelp(true);
        }
        else
        {
            if (pParamList->numberOfParameters >= 1)
            {
                uint32_t i = 0; 
                Folder    * parentFolder = NULL;
                File      * pFile = NULL;
                char      * pName = NULL;
                Arguments * pArg = NULL;

                for (i = 0; i < pParamList->numberOfParameters; i++)
                {
                    pArg = getArgumentAtIndex(pParamList->parameters, i);

                    if (pArg != NULL)
                    {
                        parentFolder = getFolderFromPath(pArg->arg); 
                        getLastNameFromPath(pArg->arg, &pName);

                        ret = searchFileOrFolderIntoPool(parentFolder, pName, &pFile, NULL, false);

                        if (ret == SUCCESS
                            && pFile != NULL)
                        {
                            ret = updateFileModificationDate(pFile, NULL);
                        }
                        else if (parentFolder != NULL) 
                        {
                            pFile = createNewFile(parentFolder, 
                                                  pName,
                                                  NULL, 
                                                  DEFAULT_PERMISSIONS, 
                                                  NULL,
                                                  NULL);

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
            }
            else
            {
                ret = INVALID_PARAMETERS;
            }
        }
    }

    return ret;
}

static int32_t runChmod(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printChmodHelp(true);
        }
        else
        {
            if (pParamList->numberOfParameters == 2)
            {
                
            }
            else
            {
                ret = INVALID_PARAMETERS;
            }
        }
    }

    return ret;
}

static int32_t runSudo(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printSudoSuHelp(true);
        }
        else
        {
            if (pParamList->numberOfParameters == 1)
            {
                
            }
            else
            {
                ret = INVALID_PARAMETERS;
            }
        }
    }

    return ret;
}

static int32_t runEcho(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        if (searchFlag(pParamList, HELP_FLAG))
        {
            printEchoHelp(true);
        }
        else
        {
            Arguments * pArg = NULL;
            uint32_t    i = 0;

            for (i = 0; i < pParamList->numberOfParameters; i++)
            {
                pArg = getArgumentAtIndex(pParamList->parameters, i);

                if (pArg != NULL)
                {
                    printf("%s ", pArg->arg); 
                }
            }
            
            printf("\n");
        }
    }

    return ret;
}

static int32_t runEdit(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {

    }

    return ret;
}

static int32_t runClean(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        cleanScreen();
    }

    return ret;
}

static int32_t runCat(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        
    }

    return ret;
}

static int32_t runExit(ParamList *pParamList)
{
    int32_t ret = SUCCESS;

    if (pParamList != NULL)
    {
        
    }

    return ret;
}

