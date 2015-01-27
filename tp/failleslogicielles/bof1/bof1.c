#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

void ok() {
     printf("Bienvenue dans HorlogeDereglee 6.0\n");
     long H;
     time(&H); // Recupere l'heure
     int heure= H % 86400 / 3600 - 1;
     int minute= H % 3600 / 60 ;
     int seconde= H % 60; 
     printf("Il est %02i:%02i:%02i\n",heure,minute,seconde);
     printf("Merci d'avoir utilise HorlogeDereglee 6.0 !\n");
}

void ko() {
     printf("Mauvais mot de passe\n");
}

void check() {
     char buffer[64];
     printf("Adresse du buffer: %x\n",buffer);
     printf("Adresse fonction acces_autorise: %x\n",ok);
     printf("Entrez le mot de passe du logiciel: ");
     gets(buffer);
     
     if(strcmp(buffer,"rtyu")==0) {
       ok();
     }
     else {
       ko();   
     }                         
}

int main(int argc,char** argv) {
    
    check();
    return 0;
}
