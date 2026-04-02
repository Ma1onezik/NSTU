/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void abort (void) {
 struct sigaction sact;
 sigset_t sset;

 /* Вытолкнем буфера */
 (void) fflush (NULL);

 /* Снимем блокировку сигнала SIGABRT */
 if ((sigemptyset (&sset) == 0) && (sigaddset (&sset, SIGABRT) == 0)) {
   (void) sigprocmask (SIG_UNBLOCK, &sset, (sigset_t *) NULL);
 }

 /* Пошлем себе сигнал SIGABRT.                     */
 /* Возможно, его перехватит функция обработки,     */
 /* и тогда вызывающий процесс может не завершиться */
 raise (SIGABRT);

 /* Установим подразумеваемую реакцию на сигнал SIGABRT */
 sact.sa_handler = SIG_DFL;
 sigfillset (&sact.sa_mask);
 sact.sa_flags = 0;
 (void) sigaction (SIGABRT, &sact, NULL);

 /* Снова пошлем себе сигнал SIGABRT */
 raise (SIGABRT);

 /* Если сигнал снова не помог, попробуем еще одно средство завершения */
 _exit (127);
}

int main10 (void) {
 printf ("Перед вызовом abort()\n");
 abort ();
 printf ("После вызова abort()\n");
 return 0;
}