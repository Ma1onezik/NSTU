//------------------ posix15 - родитель, 2 потомка - разделяемая память - массив int[] 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <time.h>

const char *memName="lab2-72.01";
int shMemId=0;
int shMemSize=100;
int childNum=1;

//--------------------------------------------------------------------	
void print(int a[], int size){
	for (int i=0; i<size; i++)
		printf("%d ",a[i]);
	printf("\n");	
	}	

int main (void) {
	printf("Получить handle ----------------------1\n");
	shMemId  = shm_open(memName, O_CREAT|O_RDWR, 0777);
	if (shMemId==-1){
		perror("Ошибка создания памяти(shm_open)");
        return 1;
		}
	printf("Определить размер ----------------------2\n");
	ftruncate(shMemId, (shMemSize+1)*sizeof(int));
	printf("Родитель: pid=%d Получить доступ--------------3\n",getpid ());
    int *addr = (int*)mmap(0, (shMemSize+1)*sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, shMemId, 0);
	printf("addr=%lx\n",(long unsigned int)addr);
    if ((char*)addr == (char*)-1 ) {
        perror("Ошибка связывания памяти (mmap)");
        return 1;
		}		
	srand(time(NULL));
	printf("Заполнить память ----------------------4\n");
	for (int i=0; i<shMemSize; i++) 
		addr[i]=rand()%(shMemSize/2);
	print(addr,shMemSize);
	int *addr2;
	int *addr3;
    switch (fork ()) {
		case -1:  
			perror ("FORK 1"); exit (2);
		case 0:	
			// Порожденный процесс
			printf("Порожденный процесс(%d): pid=%d предок=%d Получить доступ--------------5\n",childNum,getpid (),getppid());
			addr2 = (int*)mmap(0, (shMemSize+1)*sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, shMemId, 0);
			printf("addr=%lx\n",(long unsigned int)addr2);
			print(addr2,shMemSize);
            exit (0);
		default:		
			printf("Родитель: pid=%d\n",getpid ());
			childNum=2;
			switch (fork ()) {
				case -1:  
					perror ("FORK 2"); exit (2);
				case 0:	
					// Порожденный процесс
					printf("Порожденный процесс(%d): pid=%d предок=%d Получить доступ--------------5\n",childNum,getpid (),getppid());
					addr3 = (int*)mmap(0, (shMemSize+1)*sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, shMemId, 0);
					printf("addr=%lx\n",(long unsigned int)addr3);
					print(addr3,shMemSize);
					exit (0);
				default:		
					printf("Родитель: pid=%d\n",getpid ());
					}
				}
			//-------------------------------------------------------------------------------------
		wait (NULL);
		return (0);
		}