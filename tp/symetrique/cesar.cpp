#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

int main(int argc,char** argv) {
  
  ifstream fichier;
  string cle;

  if(argc<3) {
    cout<<"Usage: "<<argv[0]<<" <fichier> <cle (un caractere en minuscule)>"<<endl;
    return -1;
  }

  //Récupère les arguments
  fichier.open(argv[1],ios::in);
  if(fichier.bad()) {
    cout << "Impossible d'ouvrir "<<argv[1]<<endl;
    return -1;
  }
  cle=argv[2];
  int decalage=cle[0]-'a';

  //Lit le fichier caractère par caractère
  while(true) {
    char lettreLue;
    int codeLettreLue;
    char lettreChiffree;
    int codeLettreChiffree;

    //Lit un caractere du fichier
    fichier >> lettreLue;

    //Fin de fichier ?
    if(fichier.eof()) {
      break;
    }

    //Convertir le caractere en entier compris entre 0 et 25
    //avec 'a'=0 ... z='25'
    codeLettreLue=lettreLue-'a';
	//Ajoute le décalage (la clé)
    codeLettreChiffree=(codeLettreLue+decalage)%26;
	//Reconvertit le code du chiffré vers un 
	//caractère (en ajoutant le code ascii de 'a' qui vaut 97)
    lettreChiffree=codeLettreChiffree+'a';
    
    cout << lettreChiffree;
   
  }

  return 0;
}
