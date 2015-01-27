---------------------------------------------------------------------------
TP sécurité: attaque d'un algorithme élémentaire de chiffrement symétrique.
---------------------------------------------------------------------------
- OL - RT -

* Compilation du TP: tapper make
  cela produit une archive tp1.zip, à distribuer aux étudiants.
	
	Problèmes éventuels:
	- la version actuelle compile et fonctionne sous cygwin
	- sous linux, il se peut qu'il y ait un pb (à tester)
	  * il faut d'abord definir la constante OS (ligne 9 de tools/tools.h) par LINUX
		ligne 77 de tools/tools.c : le 38 n'est peut être pas correct en raison des
    différences mystiques du codage des fichiers cygwin / linux.
    essayer 37 à la place...		
	
	
* L'énoncé du TP est dans questions.txt.
  (à lire avant tout)

* Pour générer un fichier de clairs/chiffrés:
  - fixer la clé contenu dans la constante SECRETKEY dans cipher/Makefile
	- définir la bijection dans le fichier bijection-secrete.txt.
	- cd cipher
	- make plain-cipher.txt
	et hop !
	
---------------------------------------------------------------------------
