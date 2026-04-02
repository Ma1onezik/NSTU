/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

int main8 (void) {
    int size=100000;
    int *shm_ptr=new  int[size];		// Зачем создавать, когда получаем
    struct sembuf P = {0, -1, 0};
    struct sembuf V = {0,  1, 0};
    int shmid;
    int semid;
    sembuf p;
	//--- получить id разделяемой памяти
    shmid = shmget (IPC_PRIVATE, size*sizeof(int), 0777);
	//--- получить id семафора
    semid = semget (IPC_PRIVATE, 1, 0777);
	//--- Установка семафора
    semctl (semid, 0, SETVAL, 1);
    switch (fork ()) {
        case -1: perror ("FORK"); return (1);
        case 0:
			//--- получить адрес разделяемой области и проверить его на -1 
            if ((int) (shm_ptr = (int*) shmat (shmid, NULL, 0)) == (-1)) {
                perror ("CHILD-SHMAT"); return (2);
                }
            if (semop (semid, &p, 1) != 0) { perror ("CHILD-SEMOP-P"); return (3); }
            printf ("Процесс-потомок вошел в критический интервал\n");
            shm_ptr[0] = getpid ();         // Монопольный доступ - запись id процесса
            printf ("Процесс-потомок перед выходом из критического интервала\n");
            if (semop (semid, &V, 1) != 0) { perror ("CHILD-SEMOP-V"); return (4); }
            shmdt (shm_ptr);
            return 0;
            }
    if ((int) (shm_ptr = (int*) shmat (shmid, NULL, 0)) == (-1)) {
        perror ("PARENT-SHMAT"); return (2);
        }
    if (semop (semid, &p, 1) != 0) { perror ("PARENT-SEMOP-P"); return (3); }
    printf ("Родительский процесс вошел в критический интервал\n");
    shm_ptr[0] = getpid ();         /* Монопольный доступ */
    printf ("Родительский процесс перед выходом из критического интервала\n");
    if (semop (semid, &V, 1) != 0) { perror ("PARENT-SEMOP-V"); return (4); }
    wait (NULL);
    printf ("Идентификатор родительского процесса: %d\n", getpid ());
    printf ("Идентификатор процесса в разделяемой структуре: %d\n", shm_ptr[0]);
    shmdt (shm_ptr);
    semctl (semid, 1, IPC_RMID);
    shmctl (shmid, IPC_RMID, NULL);
    return 0;
    }
