#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>



void ko() {
     printf("Mauvais mot de passe\n");
}

void check() {
     char buffer[170];
     printf("Adresse du buffer: %x\n",buffer);
     printf("Entrez le mot de passe du logiciel : ");
     
     gets(buffer);
     if(strcmp(buffer,"rtyu")==0)
       ok();
     else
       ko();      
}

int main(int argc,char** argv) {
    
    check();
    return 0;
}
