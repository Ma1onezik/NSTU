//------------ posix13 - родитель, 2 потомка - неименованные каналы, передача массива родитель/потомок размерность/массив
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>


int main (void) {
    int fd [2];
	int fd2[2];
    int A[]={1,2,3,4,5,6,7,8,9,10};
    int A2[]={10,9,8,7,6,5,4,3,2,1};
    int B[10];
	int childNum=1;
    /* cоздадим безымянный канал */
    if (pipe (fd) < 0) 
		{ perror ("PIPE 1"); exit (1); }
    switch (fork ()) {
		case -1:  
			perror ("FORK 1"); exit (2);
		case 0:	
			// Порожденный процесс
			printf("Порожденный процесс: потомок-%d pid=%d fd=%d,%d\n",childNum, getpid (),fd[0],fd[1]);
            close (fd [1]);
            if (read (fd [0], B, sizeof(int)*10) == sizeof(int)*10) {
                for(int i=0;i<10;i++)
                    printf("%d ",B[i]);
					}
            printf("\n");
            exit (0);
		default:		
			printf("Родитель: pid=%d fd=%d,%d\n",getpid (),fd[0],fd[1]);
			close (fd [0]);
			if (write (fd [1], A, sizeof(int)*10) != sizeof(int)*10) {
				perror ("WRITE TO PIPE-1");
				}	
			close (fd [1]);
			//--------------------------- второй потомок -------------------------------------
			if (pipe (fd2) < 0) 
				{ perror ("PIPE 2"); exit (1); }
			childNum=2;
			switch (fork ()) {
				case -1:  
					perror ("FORK 2"); exit (2);
				case 0:	
					// Порожденный процесс
			printf("Порожденный процесс: потомок-%d pid=%d fd=%d,%d\n",childNum, getpid (),fd2[0],fd2[1]);
					close (fd2 [1]);
					if (read (fd2 [0], B, sizeof(int)*10) == sizeof(int)*10) {
					for(int i=0;i<10;i++)
						printf("%d ",B[i]);
						}
					printf("\n");
					exit (0);
				default:		
					printf("Родитель: pid=%d fd=%d,%d\n",getpid (),fd2[0],fd2[1]);
					close (fd2 [0]);
					if (write (fd2 [1], A, sizeof(int)*10) != sizeof(int)*10) {
						perror ("WRITE TO PIPE-2");
						}
					close (fd2 [1]);
					}
				}
			//-------------------------------------------------------------------------------------
		wait (NULL);
		return (0);
		}