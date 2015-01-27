# -*- coding: cp1252 -*-
import sys

nom_fic_in=sys.argv[1]
nom_fic_out=nom_fic_in+".fmt"


fic_in=file(nom_fic_in,"rb")
fic_out=file(nom_fic_out,"wb")

lettre=fic_in.read(1)


while lettre!="":
    lettre=lettre.lower()
    lettre=lettre.replace("à","a")
    lettre=lettre.replace("ê","e")
    lettre=lettre.replace("é","e")
    lettre=lettre.replace("è","e")
    lettre=lettre.replace("ç","c")
    lettre=lettre.replace("ù","u")
    i=ord(lettre[0])
    if (i>=ord('a') and i<=ord('z')):
        fic_out.write(lettre)
    lettre=fic_in.read(1)
    
fic_in.close()
fic_out.close()
