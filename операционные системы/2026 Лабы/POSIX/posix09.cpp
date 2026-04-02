//--------------- posix09 - родитель- 1 потомок, неименованный канал, передача массива фикс. размерности
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main (void) {
    int fd [2];
    int A[]={1,2,3,4,5,6,7,8,9,10};
    int B[10];
    /* cоздадим безымянный канал */
    if (pipe (fd) < 0) { perror ("PIPE"); exit (1); }
    switch (fork ()) {
	case -1:  perror ("FORK"); exit (2);
	case 0:
            close (fd [1]);
            if (read (fd [0], B, sizeof(int)*10) == sizeof(int)*10) {
                for(int i=0;i<10;i++)
                    printf("%d ",B[i]);
                }
            printf("\n");
            exit (0);
            }
    close (fd [0]);
    if (write (fd [1], A, sizeof(int)*10) != sizeof(int)*10) {
        perror ("WRITE TO PIPE-1");
        }
    close (fd [1]);
    wait (NULL);
    return (0);
    }