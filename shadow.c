
#define _GNU_SOURCE
//#include<stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include<pthread.h>
#include<errno.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/syscall.h>
//#include<sys/sem.h>
#include<unistd.h>

size_t MAX = 4000000;
size_t GLOBAL_SIZE = 0;


typedef int (*old_open)(const char *path, int flags, mode_t mode); 

int open(const char *path, int flags, mode_t mode){

    //#ifdef DEBUG
        fprintf(stderr, "Child process attempting to open file %s\n", path); 
   // #endif /* DEBUG */ 
/* 
    struct stat buf;
    memset(&buf, 0, sizeof(stat));

    __xstat(_STAT_VER, path, &buf);
*/
//    printf("The file is trying to use open() to access %s\n", path);
    old_open old_open_func;
    //old_open old_open_func;
    old_open_func = (old_open)dlsym(RTLD_NEXT,"open");

    pid_t caller = syscall(SYS_gettid);
//    printf("calling thread has pid %d\n", caller);
    kill(caller, 9); 

    return -1;
}

typedef int (*old_open64)(const char *path, int flags, mode_t mode); 

int open64(const char *path, int flags, mode_t mode){
   // #ifdef DEBUG
        fprintf(stderr, "Child process attempting to open file %s\n", path); 
   // #endif /* DEBUG */ 
    old_open64 old_open64_func;
    old_open64_func = (old_open64)dlsym(RTLD_NEXT,"open64");
     
    pid_t caller = syscall(SYS_gettid);
  //  printf("calling thread has pid %d\n", caller);
    kill(caller, 9); 

   return -1;
}

typedef FILE *(*old_fopen64)(const char *__restrict__filename, \
    const char *__restrict__modes); 

extern FILE *fopen64 (const char *__restrict __filename,
		    const char *__restrict __modes) {
    //#ifdef DEBUG
        fprintf(stderr, "Child process attempting to open file\n"); 
   // #endif /* DEBUG */ 
    old_fopen64 old_fopen64_func;
    old_fopen64_func = (old_fopen64)dlsym(RTLD_NEXT,"fopen64");
    pid_t caller = syscall(SYS_gettid);
    //printf("calling thread has pid %d\n", caller);
    kill(caller, 9); 


    return NULL;
}


typedef FILE *(*old_fopen)(const char *__restrict__filename, \
    const char *__restrict__modes); 

extern FILE *fopen (const char *__restrict __filename,
		    const char *__restrict __modes) {
   // #ifdef DEBUG
        fprintf(stderr, "Child process attempting to open file\n"); 
   // #endif /* DEBUG */ 
    old_fopen old_fopen_func;
    old_fopen_func = (old_fopen)dlsym(RTLD_NEXT,"fopen");
    pid_t caller = syscall(SYS_gettid);
    //printf("calling thread has pid %d\n", caller);
    kill(caller, 9); 


    return NULL;
}

typedef pid_t (*old_fork)(void);

extern pid_t fork (void){
   // #ifdef DEBUG
        fprintf(stderr, "Child process attempting to fork\n"); 
   // #endif /* DEBUG */ 

    pid_t caller = syscall(SYS_gettid);
   // printf("calling thread has pid %d\n", caller);
    kill(caller, 9); 


    return -1;
}


typedef int (*pthread_old)(pthread_t *thread, const pthread_attr_t *attr,\
    void *(routine)(void*), void *arg);

extern int pthread_create (pthread_t *__restrict __newthread,
			   const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg) {
        //__THROWNL __nonnull ((1, 3));
    //#ifdef DEBUG
        fprintf(stderr, "Child process attempting to create a thread\n"); 
    //#endif /* DEBUG */ 
    //errno = ENOMEM;
//    __set_errno(ENOMEM);

    pid_t caller = syscall(SYS_gettid);
    //printf("calling thread has pid %d\n", caller);
    kill(caller, 9); 


    return -1;
}

typedef void *(*old_malloc)(size_t size);

void *malloc(size_t size){
    void *ptr;
    if(size >= MAX){
       // #ifdef DEBUG
            fprintf(stderr, "Child wants to allocate more than 4mb heap\n"); 
       // #endif /* DEBUG */ 
        pid_t caller = syscall(SYS_gettid);
        printf("calling thread has pid %d\n", caller);
        kill(caller, 9); 


        return NULL;
    } else if (size + GLOBAL_SIZE >= MAX){
      //  #ifdef DEBUG
            fprintf(stderr, "Child wants to allocate more than 4mb heap\n"); 
       // #endif /* DEBUG */ 
        pid_t caller = syscall(SYS_gettid);
        printf("calling thread has pid %d\n", caller);
        kill(caller, 9); 


        return NULL;
    }
    GLOBAL_SIZE += size;
    old_malloc old_malloc_func;
    old_malloc_func = (old_malloc)dlsym(RTLD_NEXT, "malloc");
    ptr = old_malloc_func(size);
    return ptr;
}

//FOR func 6 set errno to EFBIG
/*
typedef int (*old_brk)(void *addr);

extern int __brk(void *addr){
    //printf("Do I (__brk) get called ever?\n");

    old_brk brk_func;
    brk_func = (old_brk)dlsym(RTLD_NEXT, "__brk");
    return brk_func;
}

typedef int (*old_brkr)(void *addr);

extern int brk(void *addr){
    //printf("Do I (brk) get called ever?\n");

    old_brkr brk_func;
    brk_func = (old_brkr)dlsym(RTLD_NEXT, "brk");
    return brk_func;
}

*/
