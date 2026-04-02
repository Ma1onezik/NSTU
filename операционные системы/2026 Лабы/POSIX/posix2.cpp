/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <sys/statvfs.h>
#include <stdio.h>

int main2 (int argc, char *argv[]) {
 struct statvfs buf;
 int i;
 char *cc="/cygdrive/d";
   if (statvfs (cc, &buf)) {
     fprintf (stderr, "\nstatvfs: не удалось получить информацию о файловой системе, содержащей файл %s\n",cc);
     return (-1);
      }
   printf ("\nstatvfs-информация о файловой системе, содержащей файл:\n");
   printf ("Размер блока файловой системы: %ld\n", buf.f_bsize);
   printf ("Базовый размер блока файловой системы: %ld\n", buf.f_frsize);
   printf ("Общее число блоков базового размера в файловой системе: %ld\n", buf.f_blocks);
   printf ("Общее число свободных блоков: %ld\n", buf.f_bfree);
   printf ("Число свободных блоков, доступных непривилегированным процессам: %ld\n", buf.f_bavail);
   printf ("Общее число описателей файлов: %ld\n", buf.f_files);
   printf ("Общее число свободных описателей файлов: %ld\n", buf.f_ffree);
   printf ("Число описателей файлов, доступных непривилегированным процессам: %ld\n", buf.f_favail);
   printf ("Идентификатор файловой системы: %ld\n", buf.f_fsid);
   printf ("Битная шкала флагов: %lx\n", buf.f_flag);
   printf ("Максимальная длина имени файла: %ld\n", buf.f_namemax);
   return 0;
}