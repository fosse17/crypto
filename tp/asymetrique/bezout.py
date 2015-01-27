
d=0
u=0
v=0
def bezout(a , b):
  global d,u,v  
  if b == 0:
     if a < 0:
        d = -a
        u = -1
        v = 0
     else:
        d = a
        u = 1
        v = 0
  else:
        if b < 0:
            bezout(a, -b)
            v = -v
        else:
            bezout(b, a % b)
            u = u + v 
            v = u - v 
            u = u - v 
            v = (v - u) * (a / b)

a=11
b=280
bezout(a,b)
print "u=%d v=%d" % (u,v)
print "au+bv=%d" % (a*u+b*v)
