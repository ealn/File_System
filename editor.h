/*
 * Editor.h
 *
 *  Created on: Dec 3, 2016
 *      Author: earrambi
 */

#ifndef EDITOR_H_

#define EDITOR_H_
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdarg.h>

typedef uint32_t uint32;

char* openEditor(const char *fileStr);

char *finalStr[1000];


#endif /* EDITOR_H */
