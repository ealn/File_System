/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Header that contains the functions to add trace in the code
 */

#include <stdarg.h>

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

#define TIME_BUF_SIZE       20

#define MEM_COMP        "MEMORY  -"
#define FILE_COMP       "FILE -"
#define FILE_POOL_COMP  "FILE_POOL -"
#define MAIN_COMP       "MAIN    -"
#define UI_COMP         "UI      -"

void traceDataError(const char *pComponent, const char *pCaller, const char *pParm, ...);
void traceDataWarning(const char *pComponent, const char *pCaller, const char *pParm, ...);
void traceDataDebug(const char *pComponent, const char *pCaller, const char *pParm, ...);
void initTrace(void);
void termTrace(void);
void getTimeStamp(char * pOutputTime);

//Macros:
#define MEM_ERROR(...)            traceDataError(MEM_COMP, __func__, __VA_ARGS__)
#define MEM_WARNING(...)          traceDataWarning(MEM_COMP, __func__, __VA_ARGS__)
#define MEM_DEBUG(...)            traceDataDebug(MEM_COMP, __func__, __VA_ARGS__)

#define FILE_ERROR(...)           traceDataError(FILE_COMP, __func__, __VA_ARGS__)
#define FILE_WARNING(...)         traceDataWarning(FILE_COMP, __func__, __VA_ARGS__)
#define FILE_DEBUG(...)           traceDataDebug(FILE_COMP, __func__, __VA_ARGS__)

#define FILE_POOL_ERROR(...)      traceDataError(FILE_POOL_COMP, __func__, __VA_ARGS__)
#define FILE_POOL_WARNING(...)    traceDataWarning(FILE_POOL_COMP, __func__, __VA_ARGS__)
#define FILE_POOL_DEBUG(...)      traceDataDebug(FILE_POOL_COMP, __func__, __VA_ARGS__)

#define MAIN_ERROR(...)           traceDataError(MAIN_COMP, __func__, __VA_ARGS__)
#define MAIN_WARNING(...)         traceDataWarning(MAIN_COMP, __func__, __VA_ARGS__)
#define MAIN_DEBUG(...)           traceDataDebug(MAIN_COMP, __func__, __VA_ARGS__)

#define UI_ERROR(...)             traceDataError(UI_COMP, __func__, __VA_ARGS__)
#define UI_WARNING(...)           traceDataWarning(UI_COMP, __func__, __VA_ARGS__)
#define UI_DEBUG(...)             traceDataDebug(UI_COMP, __func__, __VA_ARGS__)

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif 
