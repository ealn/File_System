/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Console utils
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "console_utils.h"
#include "memutils.h"

// Macros to select the OS to be used
#define WINDOWS
//#define UNIX

//Constants
#define STDIN_BUF_SIZE          4096
#define IS_PRINTABLE_CHAR(c)    (((c) > ' ') && ((c) < '}'))

void cleanScreen(void)
{
#if defined(WINDOWS)
    system("cls");
#elif defined(UNIX) 
    system("clear");
#endif
}

static void truncateStringWithControlChars(char *str, uint32_t size)
{
    int32_t i = 0;

    while (i < size)
    {
        //if the character is a control character
        if (iscntrl(str[i]))
        {
            //replace this character with a null terminator
            str[i] = '\0';

            //end loop
            break;
        }

        i++;
    }
}

int32_t getUInt32FromConsole(const char *consoleStr)
{
    uint32_t ret = 0;

    if (consoleStr != NULL)
    {
        char stdinBuf[STDIN_BUF_SIZE];
        unsigned long result = 0L;

        memset(&stdinBuf, 0, sizeof(char)*STDIN_BUF_SIZE);
        
        printf(consoleStr);

        //prevent buffer overflows with the variable stdinBuf
        fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
        truncateStringWithControlChars(stdinBuf, STDIN_BUF_SIZE);

        result = strtoul(stdinBuf, NULL, 0);

        ret = (uint32_t)result;
    }

    return ret; 
}

int32_t getInt32FromConsole(const char *consoleStr)
{
    int32_t ret = 0;

    if (consoleStr != NULL)
    {
        ret = (int32_t)getUInt32FromConsole(consoleStr); 
    }

    return ret;
}

uint16_t getUInt16FromConsole(const char *consoleStr)
{
    uint16_t ret = 0;

    if (consoleStr != NULL)
    {
        ret = (uint16_t)getUInt32FromConsole(consoleStr); 
    }

    return ret;
}

int16_t getInt16FromConsole(const char *consoleStr)
{
    int16_t ret = 0;

    if (consoleStr != NULL)
    {
        ret = (int16_t)getUInt32FromConsole(consoleStr); 
    }

    return ret;
}

uint8_t getUint8FromConsole(const char *consoleStr)
{
    uint8_t ret = 0;

    if (consoleStr != NULL)
    {
        ret = (uint8_t)getUInt32FromConsole(consoleStr); 
    }

    return ret;
}

int8_t getInt8FromConsole(const char *consoleStr)
{
    int8_t ret = 0;

    if (consoleStr != NULL)
    {
        ret = (int8_t)getUInt32FromConsole(consoleStr); 
    }

    return ret;
}

void getStringFromConsole(const char *consoleStr, char *outputStr, uint32_t maxSize)
{
    if (outputStr != NULL)
    {
        char stdinBuf[STDIN_BUF_SIZE];

        memset(&stdinBuf, 0, sizeof(char)*STDIN_BUF_SIZE);

        if (consoleStr != NULL)
        {
            printf(consoleStr); 
        }

        //prevent buffer overflows with the variable stdinBuf
        fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
        truncateStringWithControlChars(stdinBuf, STDIN_BUF_SIZE);

        memcpy(outputStr, stdinBuf, sizeof(char)*maxSize); 
    }
}

char getFirstCharFromConsole(const char *consoleStr)
{
    char ret = 0;

    if (consoleStr != NULL)
    {
        getStringFromConsole(consoleStr, &ret, 1);
    }

    return ret; 
}

bool validateIntInput(int32_t value, int32_t lowerLimit, int32_t upperLimit)
{
    bool isValid = true;

    if (value < lowerLimit || value > upperLimit)
    {
        printf("\nEntrada invalida\n");
        isValid = false;

        //waiting for a key input
        getchar();
    }

    return isValid;
}

bool getYesOrNotFromConsole(const char *consoleStr)
{
    bool ret = false;
    bool loop = false;
    char inputChar = 0;
    char inputStr[8];

    do
    {
        loop = false;

        memset(inputStr, 0, sizeof(char)*8);
        getStringFromConsole(consoleStr, inputStr, 8); 
        inputChar = inputStr[0];

        if (strlen(inputStr) == 1)   //1 character
        {
            if ((inputChar == 's' || inputChar == 'S')
                || (inputChar == 'n' || inputChar == 'N'))
            {
                if (inputChar == 's' || inputChar == 'S') 
                {
                    ret = true;
                }
            }
            else
            {
                printf("\nEntrada invalida\n");
                //waiting for a key input
                getchar();
                loop = true;
            }
        }
        else
        {
            printf("\nEntrada invalida\n");
            //waiting for a key input
            getchar();
            loop = true;
        }
    }
    while (loop);

    return ret;
}

uint8_t createMenuWithMultipleOptions(const char * title,
                                      const char * header,
                                      const char * options,
                                      const char * footer,
                                      bool needValidateInput,
                                      int32_t lowerLimit, 
                                      int32_t upperLimit, 
                                      bool needCleanScreen)
{
    uint8_t optionSelected = 0;
    bool    isValidOption = true;

    do 
    {
        if (needCleanScreen)
        {
            cleanScreen();
        }

        if (title != NULL) 
        {
            printf(title); 
        }
        if (header != NULL)
        {
            printf(header); 
        }
        if (options != NULL)
        {
            printf(options); 
        }

        if (footer != NULL)
        {
            optionSelected = getUint8FromConsole(footer); 
        }

        if (needValidateInput)
        {
            isValidOption = validateIntInput(optionSelected, lowerLimit, upperLimit); 
        }
    }while (!isValidOption);

    return optionSelected;
}

void parseString(const char *strToBeParsed, const char separator, char ***opMatrix, uint32_t *oNumberOfElements)
{
    if (strToBeParsed != NULL
        && oNumberOfElements != NULL
        && opMatrix != NULL)
    {
        uint32_t  i = 0;
        uint32_t  j = 0;
        uint32_t  k = 0;
        bool      isChar = 0;
        uint32_t  size = 0;
        uint32_t  maxSize = 0;
        char    **matrix = NULL;
        uint32_t  nRows = 0;
        uint32_t  nCol = 0;

        *oNumberOfElements = 0;
        *opMatrix = NULL;

        //get number of arguments
        for (i = 0; strToBeParsed[i] != '\0'; i++)
        {
            if (IS_PRINTABLE_CHAR(strToBeParsed[i])
                && (strToBeParsed[i] != separator))
            {
                //An argument was found
                if (!isChar)
                {
                    (*oNumberOfElements)++; 
                }

                //this variable is used to indicate if a printable char was found
                isChar = true;
                size++;

                if (size > maxSize)
                {
                    maxSize = size;
                }

                continue;
            }

            if (isChar
                && (strToBeParsed[i] == separator))
            {
                size = 0;
                isChar = false; 
            }
        }

        if (*oNumberOfElements > 0)
        {
            nRows = *oNumberOfElements;
            nCol = maxSize + 1; //add the null character

            //alocate memory for the matrix
            matrix = (char **)MEMALLOC(nRows * sizeof(char *));

            if (matrix != NULL)
            {
                //allocate memory of each argument
                for (i = 0; i < nRows; i++)
                {
                    matrix[i] = (char *)MEMALLOC(nCol * sizeof(char));
                }

                //copy the info to the array of Arguments
                for (i = 0; strToBeParsed[i] != '\0'; i++)
                {
                    if (IS_PRINTABLE_CHAR(strToBeParsed[i])
                        && (strToBeParsed[i] != separator))
                    {
                        //copy char by char the string
                        isChar = true;
                        matrix[j][k] = strToBeParsed[i];
                        k++;
                        continue;
                    }
             
                    if (isChar
                        && (strToBeParsed[i] == separator))
                    {
                        //Increase the index of the array of arguments
                        j++;
                        k = 0;
                        isChar = false;
                    }
                }

                *opMatrix = matrix;
            }
        }
    }
}

void destroyStringsParsed(char ** matrixStr, uint32_t numberOfElements)
{
    if (matrixStr != NULL
        && numberOfElements > 0)
    {
        uint32_t i = 0;

        for (i = 0; i < numberOfElements; i++)
        {
            MEMFREE(matrixStr[i]);
        }

        MEMFREE(matrixStr);
    }
}

void getArgumentsFromConsole(char * consoleStr, char *** opArguments, uint32_t *oNumberOfArgs)
{
    if (oNumberOfArgs != NULL
        && opArguments != NULL)
    {
        char outConsoleStr[STDIN_BUF_SIZE/2]; 

        memset(&outConsoleStr, 0, sizeof(char)*(STDIN_BUF_SIZE/2));

        getStringFromConsole(consoleStr, outConsoleStr, (STDIN_BUF_SIZE/2));

        //parse string to get the arguments
        parseString(outConsoleStr, ' ', opArguments, oNumberOfArgs);
    }
}



