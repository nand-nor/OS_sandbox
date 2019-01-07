/*
 * Allie Clifford
 * Comp111 Assignment 3
 * watch.c
 * Date created: 10/23/2018
 * Last modified: 11/10/2019
 * 
 * How to compile: 
 *      gcc -g watch watch.c -lpthread -lrt
 *      gcc -g -fPIC -shared shadow.c -o shadow.so -ldl
 * 
 * Case handling:
 * 1. Limit size of stack to 4 mb. 
 *      This case is handled via prlimit (sorry) and therefore
 *      child is killed by kernel via signal 11 and not via signal 9. This is
 *      the only requirement that, to my current knowledge, I 
 *      have not successfully met
 * 2. Limit size of heap to 4 mb 
 *      Limits heap size via shadow hooking  malloc
 * 3. Prohibit forking
 *      Limits forking via shadow hooking fork
 * 4. Prohibit thread creation
 *      Limits thread creation via shadow hooking pthread
 * 5. Prohibit opening files
 *      Limits file opening via shadow hooking function calls
 * 6. Limit size of global variables
 *      Limits size of initialized global variables via popen
 *      call to "size" and parsing responde for bss size
 */
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <stdint.h> 
#include <time.h>
#include <sys/time.h> 
#include <sys/resource.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <fcntl.h> 
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/times.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include<sys/stat.h>
#include<string.h>

/*define directives*/
#define TRUE 1
#define FALSE 0
#define CLOCKID CLOCK_REALTIME
#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#define SIG 11

/*global vars*/
int DONE = FALSE;
int LINES = 0;
time_t DEATH;
struct rusage RSTART = {0}, RSTOP = {0};
struct timespec TSTART = {0}, TSTOP = {0};
pid_t PARENT = 0,  CHILD = 0;

/*function declarations*/
void reaper(int sig);
void print_time_stats(char **argv);
void print_ruse_stats(char **argv);
void monitor_child(FILE *stream);
void print_info(char **argv);
int check_size(char **argv);

/*Reap child process upon completion report exit status or signal */
void reaper(int sig){ 
    if(sig == SIGINT || sig == SIGSEGV || sig == SIGKILL || sig == SIGUSR1){
        fprintf(stderr, "Child generated signal %d, killing\n", sig); 
        kill(CHILD, 9);
    } 
    int status;
    pid_t child = waitpid(0, &status, WNOHANG);  
    if(child != CHILD && child >0){
        return;
    }
    if(child > 0){
        getrusage(RUSAGE_SELF, &RSTOP);  
        clock_gettime(CLOCKID, &TSTOP); 
        time(&DEATH); 
        if(WIFEXITED(status)){
            fprintf(stderr, "Child exited with status %d\n", WEXITSTATUS(status));
        }
        if(WIFSIGNALED(status)){
            fprintf(stderr, "Child exited via signal %d\n", WTERMSIG(status));
        }
    } 
    if(child < 0){
         if(errno == ECHILD){
            fprintf(stderr,"Child error-- child does not exist\n");
            exit(EXIT_FAILURE);
        } else {
            fprintf(stderr, "Invalid args to waitpid\n");
            exit(EXIT_FAILURE);
        }
    }
    DONE=TRUE; 
    return;
}

/*monitor the child's activity*/
void monitor_child(FILE *stream){ 
    char buff[1024];
    while(!DONE && !feof(stream) && !ferror(stream) && \
        fgets(buff, sizeof(buff), stream) != NULL) {
        LINES += 1;
        fputs(buff, stderr);
    }
}

/*check bss size to limit global initialized variables*/
int check_size(char **argv){
    int text, data, bss, dec, hex;
    char *filename[256];
    char buf[256];
    char resp[256]; 
    FILE *proc; 
    sprintf(buf,"size %s",argv[1]); 
    proc = popen(buf,"r"); 
    if (proc) { 
        fgets(resp, 256, proc); 
        fgets(resp, 256, proc);
        sscanf(resp, "%d %d %d %d %s", &text, &data, &bss, &dec, &hex, &filename);
        if(bss >= 4000000){
            return 1;
        } else {
            return 0;
        }
    } else {  
        return 0; 
     }
}

/*calculate and print ruse stats from child*/
void print_ruse_stats(char **argv){
    struct rusage *rsbuf = &RSTART;
    struct rusage *rebuf = &RSTOP;
    double su=(double)rsbuf->ru_utime.tv_sec 
        + ((double)rsbuf->ru_utime.tv_usec)/1e6; 
    double ss=(double)rsbuf->ru_stime.tv_sec 
        + ((double)rsbuf->ru_stime.tv_usec)/1e6; 
    double eu=(double)rebuf->ru_utime.tv_sec 
        + ((double)rebuf->ru_utime.tv_usec)/1e6; 
    double es=(double)rebuf->ru_stime.tv_sec 
        + ((double)rebuf->ru_stime.tv_usec)/1e6; 
    fprintf(stderr,"Approximate user time (1e6) for %s: %e\n",argv[1], eu - su); 
    fprintf(stderr, "Approximate sys time (1e6) for %s: %e\n", argv[1], es - ss); 
    return;
}

/*calculate and print wallclock time of death*/
void print_time_stats(char **argv){
    struct timespec *sbuf = &TSTART;
    struct timespec *ebuf = &TSTOP;
    double s=(double)sbuf->tv_sec 
        + ((double)sbuf->tv_nsec)/1e6;  
    double e=(double)ebuf->tv_sec 
        + ((double)ebuf->tv_nsec)/1e6; 
    fprintf(stderr, "Approximate realtime (1e6) for %s: %e\n", argv[1], e - s); 
    fprintf(stderr, "Wallclock time of death: %s\n", ctime(&DEATH));
    return;
}

/*print process data*/
void print_info(char **argv){
    fprintf(stderr, "Total lines printed by child: %d\n", LINES);
    print_ruse_stats(argv);
    print_time_stats(argv);
}

int main(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr,"Useage: %s <executable file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int fds[2];
    pipe(fds);
    FILE *stream;
    struct rlimit stack_lim, old_stack;
    struct rlimit *pstack_lim = NULL;
    stack_lim.rlim_cur = 4000000;
    stack_lim.rlim_max = 4000000;
    pstack_lim = &stack_lim;
    struct sigaction sa;
    sigset_t mask; 
    sa.sa_handler = reaper;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1){
        fprintf(stderr, "SIGACTION failure: %s", strerror(errno));
        exit(EXIT_FAILURE);   
    } 
    if (sigaction(SIG, &sa, NULL) == -1){
        fprintf(stderr, "SIGACTION failure: %s", strerror(errno));
        exit(EXIT_FAILURE);   
    } 
    if (sigaction(SIGINT, &sa, NULL) == -1){
        fprintf(stderr, "SIGACTION failure: %s", strerror(errno));
        exit(EXIT_FAILURE);   
    } 
    if (sigaction(SIGSEGV, &sa, NULL) == -1){
        fprintf(stderr, "SIGACTION failure: %s", strerror(errno));
        exit(EXIT_FAILURE);   
    } 
    if (sigaction(SIGUSR1, &sa, NULL) == -1){
        fprintf(stderr, "SIGACTION failure: %s", strerror(errno));
       exit(EXIT_FAILURE);   
    } 
    if(check_size(argv)){
        fprintf(stderr, "File %s bss larger than 1mb, exiting\n", argv[1]);
        exit(EXIT_SUCCESS);
    }
    PARENT = getpid();
    if((CHILD = fork())){
        clock_gettime(CLOCKID, &TSTART);  
        getrusage(RUSAGE_SELF, &RSTART);
    } else {
        /*limit child*/ 
        if(prlimit(0, RLIMIT_STACK, pstack_lim, &old_stack) == -1){
            fprintf(stderr,"prlimit failure errno set to %d %s\n", \
                errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(1);
        dup(fds[1]);
        close(fds[1]);
        close(fds[0]);
        char *const args[] = {argv[1], NULL};
        char *const envs[]  = {"LD_PRELOAD=./shadow.so", NULL};
        if(execve(argv[1], args, envs) == -1){        
            fprintf(stderr, "Execve call failed for %s, errno set to %d %s\n",\
                 argv[1], errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }   
    stream = fdopen(fds[0],"r");
    close(fds[1]);
    monitor_child(stream);         
    print_info(argv);  
    return EXIT_SUCCESS;
}
