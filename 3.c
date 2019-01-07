// a very friendly and control-C sensitive fork botch.
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>
#include<errno.h>

int main() { 
    pid_t pid;
    if (pid = fork()) { 
        sleep(1); 
        fprintf(stderr, "i could not fork  getpid returns %d and fork PID is %d\n", getpid(), pid); 
    } else {
        sleep(1);
        fprintf(stderr, "I forked!!...errno is %d and PID is %d\n", errno, getpid());
//        fprintf(stderr, "oink! PID is %d\n", getpid()); 
    }
    return 0;
} 
