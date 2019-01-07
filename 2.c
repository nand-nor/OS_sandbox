// try to take over all memory and allocate it on your heap
#include <stdio.h> 
#include <malloc.h> 
#include <sys/types.h>
#include <unistd.h>
#include<errno.h>

int main() 
{ 
    int i, count =0; 
    for (i=0; i<1200; i++) { 
	//printf("do i even run...\n");
    //if(count + 1024 >= 400000){
    //    break;
    //}
        int *p=(int *)malloc(1024*sizeof(int)); 
        if(p != NULL){
            count += 1024*sizeof(int);
        
        } else {
            printf("p returned null\n");
            return ENOMEM;
            //break;
        } 
    } 
    fprintf(stderr, "oink! this much memory :( %d\n", count); 
    return errno;
} 
