// open a file 
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>


#define BUFLEN 1000
int main() { 
    FILE *fp = fopen("/etc/passwd", "r"); 
    char buf[BUFLEN]; 
    fgets(buf, BUFLEN, fp); 
    fprintf(stderr, "oink! %s",buf); 
    fclose(fp); 
    return 0;
} 
 
