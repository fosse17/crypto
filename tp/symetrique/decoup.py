import sys



nom_fic_in=sys.argv[1]
l=int(sys.argv[2])
fic_in=file(nom_fic_in,"rb")
position=0
fics=[]

for f in range(0,l):
    fics.append(file("prt"+str(f)+".txt","wb"))

lettre=fic_in.read(1)
while lettre!="":
    fics[position].write(lettre)
    position=(position+1) % l
    lettre=fic_in.read(1)

for f in range(0,l):
    fics[f].close()
