/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" :
 * oskar.ingemarsson[at].gmail.com wrote this file. As long as you retain this 
 * notice you can do whatever you want with this stuff. If we meet some day, and 
 * you think this stuff is worth it, you can buy me a beer in return. 
 * Oskar Ingemarsson 
 * -----------------------------------------------------------------------------
 */

#include<stdio.h>
#include<string.h>
#include "parse.h"

enum opt {COMMENT,ERROR,EMPTY,PLAYLIST};

int parse(char *path,char *musicPath){
	int status = 1;
	FILE *file ; 
	file = fopen(path,"r") ; 
	if(file == NULL){
		printf("can't open config file\n");
		return 1;
	}	
	int c ; 
	char value[500]; 
	int opt;
        int i =  1; 	
	char line[100];
       	char *tmpLine;	
	while(fgets(line,100,file)){
		tmpLine = line;
		while(*tmpLine != '\0' && (*tmpLine == '\n' || *tmpLine == '\r' || 
					*tmpLine == ' '))
		{
			tmpLine++; 	
		}
		opt = set_options(tmpLine,value);
		switch(opt){
			case ERROR : printf("error in config file on line %i\n",i) ; return 1; 
			case PLAYLIST : strcpy(musicPath,value) ; status = 0;  
			default : break ;
		} 
		i++;
	}	
	if(status){
		printf("no playlist specified\n");
	}
	return status;
}


int set_options(char* line,char *value){
	int i,j;
	char *tmpValue;

	if(*line == '#'){
		return COMMENT;
	}
	if(*line == '\0'){
		return EMPTY;
	}

	for(i = 0;i < strlen(line);i++)
	{
		if(line[i] == '=' && line[i+1] != '\0'){
			tmpValue = &line[i+1];
			tmpValue[strlen(tmpValue)-1] = '\0';
			line[i] = '\0';
			break; 
		}
	}
	if(strcmp(line,"PLAYLIST") == 0){
		if(fopen(tmpValue,"r")==NULL){
			printf("playlist file %s doesn't exist\n",tmpValue);
			return ERROR;
		} 
		else {	strcpy(value,tmpValue);
			return PLAYLIST;
		}
	}
	else {
		printf("cant find playlist file\n");
		return ERROR; 	
	}
}


