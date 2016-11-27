/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Main program
 */
 
#include <stdio.h>

#include "defines.h"
#include "shell.h"
#include "trace.h"
#include "file_system.h"

int32_t main(void)
{
    int32_t ret = SUCCESS;

    initTrace();

    ret = initFileSystem();

    if (ret == SUCCESS)
    {
        ret = runShell();
        closeFileSystem();
    }

    termTrace();
    
    return ret;
}

