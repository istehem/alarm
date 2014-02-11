/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" :
 * oskar.ingemarsson[at].gmail.com wrote this file. As long as you retain this 
 * notice you can do whatever you want with this stuff. If we meet some day, and 
 * you think this stuff is worth it, you can buy me a beer in return. 
 * Oskar Ingemarsson 
 * -----------------------------------------------------------------------------
 */

#include "progress.h"

void progressbar (char *retStr, int rowLen,double timeLeftProc) {
	getStr(retStr,rowLen,timeLeftProc); 
}

void getStr(char *retStr,int rowLen,double timeLeftProc) {
	int i; 
	retStr[0] = '|';
    int until = (int)(rowLen*timeLeftProc); 	

	for(i = 1 ; i < until; i++){
		retStr[i] = '=' ; 	
	}
	retStr[i++] = '>' ;

	for(; i < rowLen-1 ; i++){
		retStr[i] = ' '; 
	}
	retStr[rowLen-2] = '|' ; 
}
