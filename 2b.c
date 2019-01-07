// grab a HUGE heap record 
#include <stdio.h> 
#include <malloc.h> 
#include <sys/types.h>
#include <unistd.h>


int main() 
{ 
    int *p=(int *)malloc(1200*1024*sizeof(int)); 

    if(p == NULL){
        printf("its NULL\n");
    }
    printf("oink!\n"); 
    return 0;
} 
