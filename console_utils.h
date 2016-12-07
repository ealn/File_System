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
 * Porpuse: Header that contains all utils for the console
 */

#include <stdint.h>
#include <stdbool.h>

#ifndef __CONSOLE_UTILS_H__
#define __CONSOLE_UTILS_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

int32_t getUInt32FromConsole(const char *consoleStr);
int32_t getInt32FromConsole(const char *consoleStr);
uint16_t getUInt16FromConsole(const char *consoleStr);
int16_t getInt16FromConsole(const char *consoleStr);
uint8_t getUint8FromConsole(const char *consoleStr);
int8_t getInt8FromConsole(const char *consoleStr);
void getStringFromConsole(const char *consoleStr, char *outputStr, uint32_t maxSize);
char getFirstCharFromConsole(const char *consoleStr);
bool validateIntInput(int32_t value, int32_t lowerLimit, int32_t upperLimit);
void cleanScreen(void);
bool getYesOrNotFromConsole(const char *consoleStr);
uint8_t createMenuWithMultipleOptions(const char * title,
                                      const char * header,
                                      const char * options,
                                      const char * footer,
                                      bool needValidateInput,
                                      int32_t lowerLimit, 
                                      int32_t upperLimit, 
                                      bool needCleanScreen);
void getArgumentsFromConsole(const char * consoleStr, char *** opArguments, uint32_t *oNumberOfArgs);
void parseString(const char *strToBeParsed, const char separator, char ***opMatrix, uint32_t *oNumberOfElements);
void destroyStringsParsed(char ** matrixStr, uint32_t numberOfElement);
char * getPasswordFromConsole(const char * consoleStr, uint32_t size);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

#endif // !defined(__CONSOLE_UTILS_H__)
