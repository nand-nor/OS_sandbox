// blow the stack with a bounded but intentionally antisocial recursion. 
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>

int count = 0;

void oink(int i) { 
    count += (1024 * sizeof(int));
//    printf("i is %d count is %d\n",i, count);
    int garbage[4024]; // push this onto the stack. 
//#ifdef DEBUG
//    fprintf(stderr, "allocated %d bytes of stack memory\n", 1024*sizeof(int)); 
//#endif /* DEBUG */
    if(count >= 4000000){
        fprintf(stderr,"This should start changin");
        return;
    } 
    if (i>0) oink(i-1); 
} 

int main() { 

    //sleep(6);

    oink(1200); 



    fprintf(stderr, "oink!\n"); 
    return 0;
} 
