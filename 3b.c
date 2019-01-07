// a very friendly and control-C sensitive fork botch.
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>


int main() { 
    fork(); 
    printf("oink!\n"); 
    return 0;
} 
