all: main.exe

main.exe: main.asm
	tasm32.exe /ml /l /m3  main.asm main.obj
	tlink32.exe /Tpe /aa  main.obj ,main.exe,, c:\\bin\\zz.lib
	./makeex
