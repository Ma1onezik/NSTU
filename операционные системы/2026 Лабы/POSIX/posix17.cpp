//---------- posix17 - именованные каналы, родитель, 2 потомка, передача массива размерность/данные
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <cerrno>

void writeParent(char *fifo, int A[], int size){
	int writefd = open(fifo, O_WRONLY);
	if (writefd<0){
		printf("Ошибка открытия(1) fifo=%s для записи=%d\n",fifo,writefd);
		return;		
		}
	int bfSize = write (writefd, &size, sizeof(int));
	if (bfSize != sizeof(int)){
		printf("Ошибка записи(1) fifo=%s size=%d\n",fifo, bfSize);
		return;
		}
	bfSize = write(writefd, A, size*sizeof(int));
	if (bfSize != size*sizeof(int)){
		printf("Ошибка записи(2) fifo=%s size=%d\n",fifo,bfSize);
		return;
		}
	close (writefd);	
	}
void readChild(char *fifo){
	int A[20];
	int readfd = open(fifo, O_RDONLY);
	if (readfd<0){
		printf("Ошибка открытия(1) fifo=%s для чтения=%d\n",fifo,readfd);
		return;		
		}
	int size=0;
	int bfSize = read(readfd, &size, sizeof(int));
	if (bfSize != sizeof(int)){
		printf("Ошибка чтения(1) fifo=%s size=%d\n",fifo,bfSize);
		return;		
		}
	bfSize = read(readfd, A, size*sizeof(int));
	if (bfSize != size*sizeof(int)){
		printf("Ошибка чтения(2) fifo=%s size=%d\n",fifo,bfSize);
		return;		
		}
    for(int i=0;i<size;i++)
        printf("%d ",A[i]);
    printf("\n");
	close (readfd);	
	}

int main (void) {
	char sName[100];
	char sName2[100];
	const char *path="/mnt/f/YandexDisk/OS_2026/POSIX";
	sprintf(sName,"/mnt/f/posix17_1_%d",getpid());
	sprintf(sName2,"/mnt/f/posix17_2_%d",getpid());
    int A[]={1,2,3,4,5,6,7,8,9,10};
	int childNum=1;
	int fifo1 = mkfifo(sName,0777);
	int fifo2 = mkfifo(sName2,0777);
	printf("Создать именованные каналы %s=%d %s=%d errno=%d\n",sName,fifo1,sName2,fifo2,errno);
	int size=0;
    switch (fork ()) {
		case -1:  
			perror ("FORK 1"); exit (2);
		case 0:	
			// Порожденный процесс
			printf("Порожденный процесс: потомок-%d pid=%d\n",childNum, getpid ());
			readChild(sName);
            exit (0);
		default:		
			//--------------------------- второй потомок -------------------------------------
			childNum=2;
			switch (fork ()) {
				case -1:  
					perror ("FORK 2"); exit (2);
				case 0:	
					// Порожденный процесс
					printf("Порожденный процесс: потомок-%d pid=%d\n",childNum, getpid ());
					readChild(sName2);
					exit (0);
				default:		
					printf("Родитель: pid=%d\n",getpid ());
					writeParent(sName,A,5);
					writeParent(sName2,A,10);
					}
				}
			//-------------------------------------------------------------------------------------
		wait (NULL);
		return (0);
		}