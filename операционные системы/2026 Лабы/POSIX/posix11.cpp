//-------------- posix11 - передача сигнала самому себе (функция реакции)
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void fufu(int signo){
    printf("!!!!!%d\n",signo);
    }

int main (void) {
    struct sigaction sact;
    sigset_t sset;
    int MYSIG=SIGALRM;
     (void) fflush (NULL);
    /* Снимем блокировку сигнала SIGABRT */
    if ((sigemptyset (&sset) == 0) && (sigaddset (&sset, MYSIG) == 0)) {
        sigprocmask (SIG_UNBLOCK, &sset, (sigset_t *) NULL);
        }
    /* Установим реакцию на сигнал  */
    sact.sa_handler = fufu;
    sigfillset (&sact.sa_mask);
    sact.sa_flags = 0;
    sigaction (MYSIG, &sact, NULL);
     /* Пошлем себе сигнал                              */
    raise (MYSIG);
    sleep(5);
    return 0;
    }