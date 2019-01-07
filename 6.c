#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>


// this should not be allowed. 
int foo[1000000]; // 8 mb; 
int main() { 

    
    printf("kilroy was here\n"); 
    return 0;
} 
