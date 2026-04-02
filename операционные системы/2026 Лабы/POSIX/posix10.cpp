//---------- posix10 - варианты завершения на сигнал SIGABRT
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void abort (void) {
	struct sigaction sact;
	sigset_t sset;
	/* Вытолкнем буфера */
	(void) fflush (NULL);
	printf("Снимем блокировку сигнала SIGABRT\n");
	if ((sigemptyset (&sset) == 0) && (sigaddset (&sset, SIGABRT) == 0)) {
		(void) sigprocmask (SIG_UNBLOCK, &sset, (sigset_t *) NULL);
		}
	printf("Пошлем себе сигнал SIGABRT\n");                     
	/* Возможно, его перехватит функция обработки,     */
	/* и тогда вызывающий процесс может не завершиться */
	raise (SIGABRT);

	printf("Установим подразумеваемую реакцию на сигнал SIGABRT\n");
	sact.sa_handler = SIG_DFL;
	sigfillset (&sact.sa_mask);
	sact.sa_flags = 0;
	(void) sigaction (SIGABRT, &sact, NULL);

	printf("Снова пошлем себе сигнал SIGABRT\n");
	raise (SIGABRT);

	printf("Если сигнал снова не помог, попробуем еще одно средство завершения\n");
	_exit (127);
	}

int main (void) {
	printf ("Перед вызовом abort()\n");
	abort ();
	printf ("После вызова abort()\n");
	return 0;
}