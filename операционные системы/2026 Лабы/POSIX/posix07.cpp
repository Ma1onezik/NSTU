/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
void atefunc (void) {
    printf ("Ситуация перед завершением родительского процесса\n");
        system ("ps");
    }
int main (void) {
	int pid;
	int stat;
	setbuf (stdout, NULL);
	/* Зарегистрируем обработчик завершения процесса */
	if (atexit (atefunc) != 0) { perror ("ATEXIT"); exit (1); }
	/* Создадим новый процесс */
	if ((pid = fork ()) < 0) { perror ("FORK"); exit (2);} 
        else if (pid == 0) {
            /* Порожденный процесс - Выполним служебную программу ps */
            printf ("Ситуация с точки зрения порожденного процесса\n");
            execl ("/bin/ps", "ps", (char *) 0);
            perror ("EXEC");
            exit (3); /* execl() завершился неудачей */
            } else {
		/* Родительский процесс */
		sleep (1);
		/* Вероятно, порожденный процесс уже завершился */
		/* Посмотрим на текущую ситуацию */
		printf ("Ситуация перед вызовом waitpid() в родительском процессе\n");
		system ("ps");
		waitpid (pid, &stat, 0);
		printf ("Статус завершения порожденного процесса с идентификатором %d: %d\n", pid, stat);
                }
	return 0;
    }