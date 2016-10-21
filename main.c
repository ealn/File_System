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
#include "trace.h"
#include "UI.h"
#include "file_pool.h"

int32_t main(void)
{
    int32_t ret = SUCCESS;

    initTrace();

    MAIN_DEBUG("\n************************** Program started ******************************\n\n");

    ret = createFilePool();
    
    if (ret == SUCCESS)
    {
        ret = showUI();
        destroyFilePool();
    }
    else
    {
        MAIN_ERROR("File pool could not be created\n");
    }
    
    MAIN_DEBUG("\n******************* Program ended with rc= %d ***************************\n\n", ret);
    
    termTrace();
    
    return ret;
}

