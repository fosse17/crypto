import sys

nom_fic_in=sys.argv[1]
l=int(sys.argv[2])
fic_in=file(nom_fic_in,"rb")

freq=[]
for f in range(0,l):
    freq.append({})
    for e in "abcdefghijklmnopqrstuvwxyz":
        freq[f][e]=0

n=[0]*l
ic={}
position=0
lettre=fic_in.read(1)
while lettre!="":
    freq[position][lettre]+=1
    n[position]+=1
    position=(position+1) % l
    lettre=fic_in.read(1)

for f in range(0,l):
    icc=0
    for e in "abcdefghijklmnopqrstuvwxyz":
        fr=float(freq[f][e])
        nr=float(n[f])
        tmp=(fr*fr) / (nr*nr)
        icc+=tmp
    ic[f]="%.3f" % icc
fic_in.close()
print ic
