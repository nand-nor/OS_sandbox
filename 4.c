/* invoking and communicating with a thread */ 
#include <stdio.h> 
#include <string.h> 
#include <pthread.h> 
#include <sys/types.h>
#include <unistd.h>
#include<sys/syscall.h>
#include <errno.h>

void *threaded_routine (void * v) { 
    //sleep(5); 
    pid_t tid = syscall(SYS_gettid);
    fprintf(stderr, "oink! thread ID = %d\n", tid); 
    return NULL;  
} 

int main()
{ 
   pthread_t thread; 
   void *retptr; 
    int retval;
   if ((retval = pthread_create(&thread, NULL, threaded_routine, (void *)NULL)) == 0) { 
	    pthread_join(thread,(void **)&retptr); 
   
    }// else { 
	 //   fprintf(stderr,"Func returned %d and error description is : %s\n",retval, strerror(errno));
     //   fprintf(stderr, "could not create thread!\n"); 
  // } 
  //  sleep(1);
    fprintf(stdout, "stdout test...\n");
    printf("derpy doop\n");
    return retval;
} 
