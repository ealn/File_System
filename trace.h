/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Header that contains the functions to add trace in the code
 */

#include <stdarg.h>

#ifndef __TRACE_H__
#define __TRACE_H__

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

#define TIME_BUF_SIZE       20

#define MEM_COMP        "MEMORY    -"

void traceDataError(const char *pComponent, const char *pCaller, const char *pParm, ...);
void traceDataWarning(const char *pComponent, const char *pCaller, const char *pParm, ...);
void traceDataDebug(const char *pComponent, const char *pCaller, const char *pParm, ...);
void initTrace(void);
void termTrace(void);
void getTimeStamp(char * pOutputTime);

//Macros:
#ifdef DEBUG
    #define MEM_ERROR(...)            traceDataError(MEM_COMP, __func__, __VA_ARGS__)
    #define MEM_WARNING(...)          traceDataWarning(MEM_COMP, __func__, __VA_ARGS__)
    #define MEM_DEBUG(...)            traceDataDebug(MEM_COMP, __func__, __VA_ARGS__)
#else
    #define MEM_ERROR(...)
    #define MEM_WARNING(...)
    #define MEM_DEBUG(...)
#endif

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif 

#endif // !defined(__TRACE_H__)

