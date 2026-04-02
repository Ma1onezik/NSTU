//----------------- posix01 - данные текущего пользователя
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
/* Печать элемента базы данных пользователей */
static void print_pwent (const struct passwd *pw) {
 printf ("Имя пользователя: %s\n", pw->pw_name);
 printf ("Идентификатор пользователя: %o\n", pw->pw_uid);
 printf ("Идентификатор группы: %o\n", pw->pw_gid);
 printf ("Начальный каталог: %s\n", pw->pw_dir);
 printf ("Начальная программа: %s\n", pw->pw_shell);
}
int main (void) {
 char *lgnm;           /* Имя текущего пользователя */
 struct passwd *pw;    /* Данные о текущем пользователе */
 /* Поиск и печать информации о текущем пользователе */
 if ((lgnm = getlogin ()) == NULL || (pw = getpwnam (lgnm)) == NULL) {
   fprintf (stderr, "\nНе удалось найти информацию о текущем пользователе\n");
   return 1;
 }
 printf ("\nИнформация о текущем пользователе\n");
 print_pwent (pw);
 /* То же для пользователя root */
 if ((pw = getpwuid ((uid_t) 0)) == NULL) {
   fprintf (stderr, "\nНе удалось найти информацию о пользователе root\n");
   return 1;
 }
 printf ("\nИнформация о пользователе root\n");
 print_pwent (pw);
 return 0;
}