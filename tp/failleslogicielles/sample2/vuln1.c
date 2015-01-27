/* gcc -g -o vuln1 vuln1.c */
int main(int argc, char **argv)
{
char buffer[100];
if (argc > 1)
     strcpy(buffer, argv[1]);
return (0);
}
