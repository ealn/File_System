/*
 * Copyright (c) 2016 by Efrain Adrian Luna Nevarez
 *                       Emmanuel Salcido Maldonado
 *                       Jesus Eduardo Silva Padilla
 *                       Efrain Arrambide Barron
 *                       Ricardo Isaac Gonzalez Ordaz
 * All Rights Reserved
 *
 * Authors: Efrain Adrian Luna Nevarez
 *          Emmanuel Salcido Maldonado
 *          Jesus Eduardo Silva Padilla
 *          Efrain Arrambide Barron
 *          Ricardo Isaac Gonzalez Ordaz
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

    ret = initFileSystem();

    if (ret == SUCCESS)
    {
        ret = runShell();
        closeFileSystem();
    }
    
    return ret;
}

