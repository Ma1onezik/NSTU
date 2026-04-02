//------------- posix16 - родитель, 2 потомка, ожидане завершения на 2 семаформах
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
#include <sys/time.h>
#include <semaphore.h>

timeval start_tv;
//--------------------------------------------------------------------	
double diffMS(timeval &end){
	return end.tv_sec * 1. + end.tv_usec/1000000. 
	- start_tv.tv_sec * 1. - start_tv.tv_usec/1000000.;
	}

int main (void) {
	timeval end;
	gettimeofday(&start_tv, NULL);
	char sName[100];
	char sName2[100];
	sprintf(sName,"/posix16(1)_%d",getpid());
	sprintf(sName2,"/posix16(2)_%d",getpid());
	printf("Создать семафоры %s %s\n",sName,sName2);
	// Создать с проверкой на уникальность, имя -  posix16_<pid>, значение 0 
	sem_t *semParent = sem_open(sName, O_CREAT | O_EXCL, 0777, 0);	
	sem_t *semParent2 = sem_open(sName2, O_CREAT | O_EXCL, 0777, 0);	
	sem_t *semChild1;
	sem_t *semChild2;
	int vv=0,vv2=0;
	if (semParent==SEM_FAILED){
		printf("Ошибка создания семафора родителем (sem_open) %s\n",sName);
        return 1;
		}
	if (semParent2==SEM_FAILED){
		printf("Ошибка создания семафора родителем (sem_open) %s\n",sName2);
        return 1;
		}
	sem_getvalue(semParent,&vv);	
	sem_getvalue(semParent2,&vv2);	
	printf("Значение семафоров (родитель)=%d,%d\n",vv,vv2);
    switch (fork ()) {
		case -1:  
			perror ("FORK 1"); exit (2);
		case 0:	
			// Порожденный процесс
			semChild1  = sem_open(sName, 0);	
			if (semChild1==SEM_FAILED){
				printf("Ошибка получения семафора потомком(1) (sem_open) %s\n",sName);
				return 1;
				}
			sem_getvalue(semChild1,&vv);
			sleep(5);	
			printf("Значение семафора потомок(1) до V-операции =%d\n",vv);
			sem_post(semChild1);
			sem_getvalue(semChild1,&vv);	
			gettimeofday(&end, NULL);
			printf("Значение семафора потомок(1) после V-операции =%d t=%5.2f\n",vv,diffMS(end));
            exit (0);
		default:		
			printf("Родитель: pid=%d\n",getpid ());
			switch (fork ()) {
				case -1:  
					perror ("FORK 2"); exit (2);
				case 0:	
					// Порожденный процесс
					semChild2  = sem_open(sName2, 0);	
					if (semChild2==SEM_FAILED){
						printf("Ошибка получения семафора потомком(1) (sem_open) %s\n",sName);
						return 1;
						}
					sem_getvalue(semChild2,&vv);	
					sleep(10);	
					printf("Значение семафора потомок(2) до V-операции =%d\n",vv);
					sem_post(semChild2);
					sem_getvalue(semChild2,&vv);	
					gettimeofday(&end, NULL);
					printf("Значение семафора потомок(1) после V-операции =%d t=%5.2f\n",vv,diffMS(end));
					exit (0);
				default:		
					sem_wait(semParent);							// P-операция
					sem_wait(semParent2);							// P-операция
					sem_getvalue(semParent,&vv);	
					sem_getvalue(semParent2,&vv2);	
					gettimeofday(&end, NULL);
					printf("Значение семафора родитель после деблокировки =%d,%d t=%5.2f\n",vv,vv2,diffMS(end));
					sem_destroy(semParent);
					}
				}
			//-------------------------------------------------------------------------------------
		wait (NULL);
		return (0);
		}