import sys

nom_fic_in=sys.argv[1]

fic_in=file(nom_fic_in,"rb")

freq={}
for e in "abcdefghijklmnopqrstuvwxyz":
    freq[e]=0

lettre=fic_in.read(1)
while lettre!="":
    freq[lettre]+=1
    lettre=fic_in.read(1)

fic_in.close()
print freq
