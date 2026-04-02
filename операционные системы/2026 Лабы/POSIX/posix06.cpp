/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main (void) {
	uid_t uid;
	int nsupgroups;
	gid_t *supgroupslist;
	int i;
	setbuf (stdout, NULL);
	printf ("Идентификаторы пользователя текущего процесса:\n" " реальный: %d, действующий: %d\n", 
                uid = getuid (), geteuid ());
	printf ("Идентификаторы группы текущего процесса:\n" " реальный: %d, действующий: %d\n",
                getgid (), getegid ());
	/* Попробуем переустановить идентификатор пользователя процесса */
	if (setuid ((uid_t) 1) != 0) { perror ("setuid (1)"); }
	printf ("Идентификаторы пользователя текущего процесса после первой смены:\n"
		" реальный: %d, действующий: %d\n", getuid (), geteuid ());
	/* Попробуем вернуть прежний идентификатор пользователя процесса */
	if (setuid (uid) != 0) { perror ("setuid (uid)"); }
	printf ("Идентификаторы пользователя текущего процесса после второй смены:\n"
		" реальный: %d, действующий: %d\n", getuid (), geteuid ());
	/* Попробуем сменить действующий идентификатор с помощью функции seteuid() */
	if (seteuid ((uid_t) 1) != 0) { perror ("seteuid (1)"); }
	printf ("Идентификаторы пользователя текущего процесса после третьей смены:\n"
		" реальный: %d, действующий: %d\n", getuid (), geteuid ());
	return (0);
}