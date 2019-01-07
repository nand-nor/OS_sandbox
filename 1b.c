// blow the stack with a really large allocation.
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>

void oink() { 
    int garbage[1200*1024]; // push this onto the stack. 
    printf("oink!\n"); 
} 

int main() { 
    oink(); 
    return 0;
} 
