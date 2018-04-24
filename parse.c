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

#define FOREACH_OPT(opt) \
        opt(COMMENT)   \
        opt(ERROR)  \
        opt(EMPTY)   \
        opt(PLAYLIST)  \
        opt(PLAY_COMMAND)  \
        opt(ARGS)  \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum opt {
    FOREACH_OPT(GENERATE_ENUM)
};

static const char *OPT_STRING[] = {
    FOREACH_OPT(GENERATE_STRING)
};

int parse(char *path, char *playCommand, char *playArgs, char *musicPath){
    int status = 1;
    FILE *file ;
    file = fopen(path,"r") ;
    if(file == NULL){
        printf("can't open config file\n");
        return 1;
    }
    char value[512];
    enum opt selOpt;
    int i =  1;
    char line[512];
    char *tmpLine;

    strcpy(musicPath, "");
    strcpy(playArgs, "");
    strcpy(playCommand, "");


    while(fgets(line,512,file)){
        tmpLine = line;
        while(*tmpLine != '\0' && (*tmpLine == '\n' || *tmpLine == '\r' ||
                    *tmpLine == ' '))
        {
            tmpLine++;
        }
        selOpt = set_options(tmpLine,value);
        switch(selOpt){
            case ERROR    : printf("error in config file on line %i\n",i) ; return 1;
                            break;
            case PLAYLIST : strcpy(musicPath,value);
                            break;
            case PLAY_COMMAND : strcpy(playCommand, value);
                            break;
            case ARGS : strcpy(playArgs,value);
                            break;
            default       : break;
        }
        i++;
    }
    if(musicPath == NULL || strcmp(musicPath,"") == 0){
        printf("no playlist specified\n");
    }
    else if (playCommand == NULL || strcmp(playCommand,"") == 0)
    {
        printf("no program for playing your music specified\n");
    }
    else {
        status = 0;
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
    if(strcmp(line,OPT_STRING[PLAYLIST]) == 0){
        if(fopen(tmpValue,"r")==NULL){
            printf("playlist file %s doesn't exist\n",tmpValue);
            return ERROR;
        }
        else {
            strcpy(value,tmpValue);
            return PLAYLIST;
        }
    }
    else if(strcmp(line,OPT_STRING[PLAY_COMMAND]) == 0){
        strcpy(value,tmpValue);
        return PLAY_COMMAND;
    }
    else if(strcmp(line,OPT_STRING[ARGS]) == 0){
      strcpy(value,tmpValue);
      return ARGS;
    }
    else
    {
      printf("unknown config option\n");
      return ERROR;
    }
}


