BIN=../bin
BASE=..
TOOLS=../tools
SECRETKEY=TATAYOYO

sym-cipher.exe: $(TOOLS)/des_gnupg.o $(TOOLS)/tools.o main.c
	gcc -I$(TOOLS) -o sym-cipher.exe $(TOOLS)/des_gnupg.o $(TOOLS)/tools.o main.c
	cp sym-cipher.exe $(BIN)

plain-cipher.txt: sym-cipher.exe
	$(BIN)/sym-cipher.exe --new-plain-cipher $(SECRETKEY) > plain-cipher.txt
	mv plain-cipher.txt $(BASE)
	
clean:
	rm -f *.exe

