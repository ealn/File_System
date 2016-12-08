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
 * Porpuse: Text Editor
 */

#include "editor.h"

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>

typedef uint32_t uint32;
char *finalStr[1000];

char* openEditor(const char *fileStr)
{
	char   *outStr;
	char   cmd[15];
	char   *end;
	size_t len;
	size_t fileLen;

	uint32 i = 0;

	if (fileStr != NULL)
	{
		fileLen = strlen(fileStr);
		while(1){
			end = strchr(fileStr, '\n');
			if (!end) {
				break;
			}
			len = strcspn(fileStr, "\n");
			finalStr[i] = (char*)malloc(len + 1);
			strncpy(finalStr[i],fileStr, len);

			*(finalStr[i] + len) = ' ';
			*(finalStr[i] + len+1) = '\0';
			printf("%d   %s \n",i, finalStr[i]);
			if ((end - fileStr) < fileLen) {
				fileStr = end + 1;
			}
			i++;
		}
	}

	while(1){
		printf("\n Press e <line_number> to edit"
			   "\n Press q to close the file"
			   "\n Press i <line number> or just i to insert a new line \n");
		fgets(cmd, 15, stdin);
		if (cmd[0] == 'e')
		{
			int j = 1;
			while(1){
				if (cmd[j] == '\n')
				{
					break;
				}
				j++;
			}
			if (j >= 1000)
			{
				printf("Invalid argument files can't have more than 1000 lines");
			}
			else
			{
				size_t linLen;
				char lineData[300];
				char *num = &cmd[2];
				size_t length = strcspn(num,"\n");
				*(num + length) = '\0';
				j = atoi(num);

				fgets(lineData, 300, stdin);
				linLen = strcspn(lineData, "\n");
				free(finalStr[j]);
				finalStr[j] = (char *)malloc(linLen + 1);
				strncpy(finalStr[j], lineData, linLen);
				*(finalStr[j] + linLen) = ' ';
				*(finalStr[j] + linLen +1) = '\0';
				memset(&lineData, '\0',300);
				int k;
				for (k = 0; k < i; k++)
				{
					printf("%d   %s \n",k, finalStr[k]);
				}
			}

		}
		else if(cmd[0] == 'q')
		{
			int m,n;
			size_t finalLen = 0;
			for (m = 0; m < i; m++)
			{
				finalLen += strlen(finalStr[m]);
			}
			outStr = (char*)malloc(finalLen + 1);
			n = 0;
			for (m = 0; m < i; m++)
			{
				memcpy(outStr + n,finalStr[m], strlen(finalStr[m])+1);
				*(outStr + (n+ strlen(finalStr[m])-1)) = '\n';
				n+= strlen(finalStr[m]);
				//free(finalStr[m]);
			}
			*(outStr + n) = '\0';
			break;
		}
		else if (cmd[0] == 'i')
		{
			int j = 1;
			int numLine = 0;
			while(1){
				if (cmd[j] == '\n')
				{
					break;
				}
				j++;
			}
			if (j >= 1000)
			{
				printf("Invalid argument files can't have more than 1000 lines");
			}
			else
			{
				char lineData[300];
				char *num = &cmd[2];
				size_t length = strcspn(num,"\n");
				*(num + length) = '\0';
				if( strlen(num) != 0){
					numLine = atoi(num);
				} else {
					numLine = i;
				}
				if (numLine > (i +1))
				{
					printf("Line doesn't exist plase try with a numer below %d",i);
					continue;
				}
				int e;
				for (e = i; e > numLine; e--)
				{
					finalStr[e] = (char *)malloc(strlen(finalStr[e-1])+1);
					memset(finalStr[e],0,strlen(finalStr[e-1])+1);
					memcpy(finalStr[e],finalStr[e-1],strlen(finalStr[e-1]));
					free(finalStr[e-1]);
				}

				fgets(lineData, 300, stdin);
				length = strcspn(lineData,"\n");
				if (e == i){
					finalStr[e] = (char *)malloc(length+1);
				}
				else{
					finalStr[numLine] = (char *)malloc(length+1);
				}
				memcpy(finalStr[numLine],lineData,length);
				*(finalStr[numLine] + length) = ' ';
				*(finalStr[numLine] + length+1) = '\0';
				i++;
				int k;
				for (k = 0; k < i; k++)
				{
					printf("%d   %s \n",k, finalStr[k]);
				}
			}

		}else
		{
			printf("Wrong option");
		}
	}
	return outStr;
}
