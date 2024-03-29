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
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

#include "alarm.h"
#include "parse.h"
#include "progress.h"

static WINDOW *mainwnd ;
int row, col ;
static char *msg = "Time until alarm rings";
int totTime;


void runTimer(int alarmTime, char *playCommand, char *playArgs, char *musicPath)
{
    int i = 0;

    mainwnd = initscr();                /* Start curses mode          */
    noecho();
    raw();
    curs_set(0);
    nodelay(mainwnd,1);
    setLayout(alarmTime) ;

    for(;alarmTime > 0;)
    {

        if(is_term_resized(row,col))
        {
            clear();
            setLayout(alarmTime);

        }
        switch(getch())
        {
            case 'q'  : curs_set(1);
                        endwin();
                        exit(0);

            case 'h'  : alarmTime = alarmTime + 3600;
                        totTime = alarmTime;
                        runTimer(alarmTime, playCommand, playArgs, musicPath);

            case 'm'  : alarmTime = alarmTime + 60;
                        totTime = alarmTime;
                        runTimer(alarmTime, playCommand, playArgs, musicPath);

            case 'H'  : if (alarmTime - 3600 > 0)
                        {
                            alarmTime = alarmTime - 3600;
                            totTime = alarmTime;
                            runTimer(alarmTime, playCommand, playArgs, musicPath);
                        }

            case 'M'  : if (alarmTime - 60 > 0)
                        {
                            alarmTime = alarmTime - 60;
                            totTime = alarmTime;
                            runTimer(alarmTime, playCommand, playArgs, musicPath);

                        }
            case '\n' : break;

            default   : break;
        }
        if(i == 10)
        {
            mvprintw(2,((col/2)-4),"%02i:%02i:%02i",
                    (alarmTime/3600)%24 , (alarmTime/60) % 60, alarmTime%60);
            char bar[10000];
            double procent = (double)alarmTime/totTime;
            progressbar(bar,col-2,1 - procent);
            mvprintw(row-3,0,"%02i%%",(int)(100 - procent*100));
            mvprintw(row-3,3,"%s",bar);
            refresh();
            alarmTime--;
            i = 0;
        }
        usleep(100000);
        i++;
    }
    endwin();               /* End curses mode        */
    if(musicPath == NULL)
    {
        while(1)
        {
            system("exec beep");
            usleep(100000);
        }
        exit(0);
    }
    char sysCmd[1024];
    strcpy(sysCmd,"exec ");
    strcat(sysCmd, playCommand);
    strcat(sysCmd, " ");
    strcat(sysCmd, playArgs);
    strcat(sysCmd, " ");
    strcat(sysCmd,musicPath);
    #if DEBUG
        printf("executing play command: %s\n", sysCmd);
    #endif
    int ret = system(sysCmd);
    exit(ret);
}

void setLayout(int alarmTime)
{
    getmaxyx(stdscr,row,col);
    mvprintw(0,(col-strlen(msg))/2,"%s",msg);
    mvprintw(2,((col/2)-4),"%02i:%02i:%02i",
        (alarmTime/3600) % 24 , (alarmTime/60) % 60, alarmTime % 60);
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    rawtime = time ( &rawtime ) + alarmTime;
    timeinfo = localtime ( &rawtime );
    strftime (buffer,80,"%c",timeinfo);
    int msglen = strlen(buffer)+16;
    mvprintw(4,(col-msglen)/2,"This will be at %s", buffer);

    char bar[10000];
    double procent = (double)alarmTime/totTime;
    progressbar(bar,col-2,1 - procent);
    mvprintw(row-3,0,"%02i%%",(int)(100 - procent*100));
    mvprintw(row-3,3,"%s",bar);
    mvprintw(row-1,0,"%s",
        "quit: q | add hour: h | subtract hour: H | add minute m | subtract minute: M");
    refresh();
}

int main (int argc, char **argv)
{
    if(argc < 3 || argc > 4)
    {
        printf("%s\n","usage: alarm [hours] [minutes | [hours] [minutes] [config file]");
        exit(1);
    }
    char musicPath[512] ;
    char playCommand[128] ;
    char playArgs[256] ;
    int res = argc == 3 ? parse("config",playCommand, playArgs, musicPath) :
                parse (argv[3], playCommand, playArgs, musicPath) ;

    #if DEBUG
        printf("player is: %s\n", playCommand);
        printf("play args is: %s\n", playArgs);
        printf("music path is: %s\n", musicPath);
    #endif

    if(res){
       printf("Config error occured\n");
       exit(1);
    }

    char sysCmd[1024];
    strcpy(sysCmd, "type ");
    strcat(sysCmd, playCommand);

    if(system(sysCmd))
    {
       printf("Specified play program %s not installed \n", playCommand);
       exit(1) ;
    }

    int seconds;
    seconds = atoi(*(argv+1))*3600  + atoi(*(argv+2))*60 ;
    totTime = seconds;
    runTimer(seconds,playCommand, playArgs, musicPath);
    return 0;
}

