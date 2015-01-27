BIN=../bin
BASE=..
TOOLS=../tools

attack.exe: $(TOOLS)/des_gnupg.o $(TOOLS)/tools.o main.c
	gcc -I$(TOOLS) -o attack.exe $(TOOLS)/des_gnupg.o $(TOOLS)/tools.o main.c 
	cp attack.exe ../bin
	
decode: always
	cat ../plain-cipher.txt | ./attack.exe
	
clean:
	rm -f *.exe

always:
