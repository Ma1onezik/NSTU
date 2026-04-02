/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
int main5 (void) {
    pid_t ppid;
    pid_t pgid;
    /* Отменим буферизацию стандартного вывода */
    setbuf (stdout, NULL);
    printf ("Атрибуты текущего процесса: pid: %d, ppid: %d, pgid: %d\n", 
            getpid (), ppid = getppid (), pgid = getpgrp ());
    /* Выделимся в новую группу */
    if (setpgid (0, 0) != 0) { perror ("setpgid (0, 0)"); }
    printf ("Новая группа текущего процесса: %d\n",
    getpgrp ());
    /* Попробуем создать новый сеанс */
    if (setsid () == (pid_t) (-1)) { perror ("setsid от имени лидера группы"); }
    /* Вернемся в прежнюю группу */
    if (setpgid (0, pgid) != 0) {
	perror ("setpgid (0, pgid)");
	}
    printf ("Группа текущего процесса после повторной смены: %d\n", getpgrp ());
    /* Повторим попытку создать новый сеанс */
    if (setsid () == (pid_t) (-1)) { perror ("setsid от имени не-лидера группы"); }
    printf ("Группа текущего процесса после создания нового сеанса: %d\n", getpgrp ());
    /* Попробуем сменить группу родительского процесса */
    if (setpgid (ppid, 0) != 0) { perror ("setpgid (ppid, 0)"); }
    /* Попробуем установить несуществующий идентификатор группы процессов */
    if (setpgid (0, ppid) != 0) { perror ("setpgid (0, ppid)"); }
    return (0);
    }