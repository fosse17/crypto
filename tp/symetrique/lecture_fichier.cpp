#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

int main(int argc,char** argv) {
  
  ifstream fichier;
  string cle;

  if(argc<3) {
    cout<<"Usage: "<<argv[0]<<" <fichier> <cle>"<<endl;
    return -1;
  }

  //Récupère les arguments
  fichier.open(argv[1],ios::in);
  if(fichier.bad()) {
    cout << "Impossible d'ouvrir "<<argv[1]<<endl;
    return -1;
  }
  cle=argv[2];

  //Lit le fichier caractère par caractère
  while(true) {
    char lettreLue;
    int codeLettreLue;

    //Lit un caractere du fichier
    fichier >> lettreLue;

    //Fin de fichier ?
    if(fichier.eof()) {
      break;
    }

    //Convertir le caractere en entier compris entre 0 et 25
    //avec 'a'=0 ... z='25'
    codeLettreLue=lettreLue-'a';
    
    cout<<"J'ai lu la lettre "<<lettreLue<<" (code="<<codeLettreLue<<")"<<endl;

    //A vous maintenant !
  }

  return 0;
}
