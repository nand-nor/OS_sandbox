/* invoking and communicating with a thread */ 
#include <stdio.h> 
#include <string.h> 
#include <pthread.h> 
#include <sys/types.h>
#include <unistd.h>


void *threaded_routine (void * v) { 
    fprintf (stderr, "oink!\n"); 
    return (void *) NULL; 
} 

int main()
{ 
    pthread_t thread; 
    void *retptr; 
    if (pthread_create( &thread, NULL, threaded_routine, NULL)==0) { 
        pthread_join(thread,(void **)&retptr); 
    } 
    return 0;
} 
