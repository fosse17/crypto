import sys


nom_fic_in=sys.argv[1]
nom_fic_out=nom_fic_in+".dec"
cle=sys.argv[2]

fic_in=file(nom_fic_in,"rb")
fic_out=file(nom_fic_out,"wb")

taille_cle=len(cle)

position=0

lettre=fic_in.read(1)
while lettre!="":
    car=ord(lettre[0])-ord('a')
    car_cle=ord(cle[position])-ord('a')
    car_crypte=(car+car_cle) % 26
    lettre_cryptee=chr(car_crypte+ord('a'))
    fic_out.write(lettre_cryptee)
    lettre=fic_in.read(1)
    position=(position+1) % taille_cle
    
fic_in.close()
fic_out.close()







