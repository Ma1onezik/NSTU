//--------------posix12 - родитель, 2 потомка, порождение процессов
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main (void) {
	int childNum=1;
	int childId=fork();
    switch (childId) {
		case -1:  
			perror ("FORK 1"); exit (2);
		case 0:	
			// Порожденный процесс
			printf("Порожденный процесс(%d): родитель=%d потомок=%d\n",childNum, getppid(), getpid ());
            exit (0);
		default:		
			printf("Родитель: pid=%d потомок=%d\n",getpid (),childId);
			//--------------------------- второй потомок -------------------------------------
			childNum=2;
			switch (fork ()) {
				case -1:  
					perror ("FORK 2"); exit (2);
				case 0:	
					// Порожденный процесс
					printf("Порожденный процесс(%d): родитель=%d потомок=%d\n",childNum, getppid(), getpid ());
					exit (0);
				default:		
					printf("Родитель: pid=%d потомок=%d\n",getpid (),childId);
					}
				}
			//-------------------------------------------------------------------------------------
		wait (NULL);
		return (0);
		}