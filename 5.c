// open a file 
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>
#include<stdlib.h>
#include<errno.h>
#define BUFLEN 1000
int main() { 
//        printf("my env: %s\n", getenv("LD_PRELOAD"));

    char *path = "/etc/passwd";
    FILE *fp = fopen(path, "r"); 
    sleep(1); 
    char buf[BUFLEN];
    if(fp == NULL){
        fprintf(stderr, "file pointer to %s is null\n", path);
        return errno;
    } 
    while (! feof(fp)) { 
    	fgets(buf, BUFLEN, fp); 
	    printf("%s",buf); 
        fprintf(stderr, "oink!\n"); 
    }
    return 0;
} 
 
