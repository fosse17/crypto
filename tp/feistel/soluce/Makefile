
all: tools cipher attack 

attack: always
	(cd tools && make)
	(cd attack && make)
	
cipher: always
	(cd tools && make)
	(cd cipher && make)
	
clean: always
	(cd bin && rm -f *.exe)
	(cd cipher && make clean)
	(cd tools && make clean)
	(cd attack && make clean)
	rm -rf tp1
	rm -rf tp1.zip
	
paquet: attack cipher
	rm -rf tp1/*
	rm -f tp1.zip
	mkdir tp1
	cp -rf tools tp1
	cp -rf attack tp1
	rm tp1/attack/soluce.c
	cp -r cipher tp1
	cp -r bin tp1
	cp bijection.txt tp1
	cp Makefile tp1
	cp plain-cipher.txt tp1
	cp questions.txt tp1
	(cd tp1 && make clean)
	zip -r tp1.zip tp1
	
always:
