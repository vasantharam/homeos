include ../../makefile.inc
.PHONY: all
all: i386.so
.PHONY: clean
clean : 
	rm -Rf *.o; rm -Rf *.so boot strt glbl
i386.so: *.c *.s boot strt glbl
	$(CC) -Di386 -shared -I"internal-include" -I"interface-include" -I../../ -I../../include *.c *.s -o i386.so; $(CC) -c -Di386 -I"internal-include" -I"interface-include" -I../../ -I../../include *.c *.s; rm -Rf boot.o strt.o glbl.o
boot : boot.o
	ld86 -d boot.o -o boot
strt : strt.o
	ld86 -d strt.o -o strt
glbl : glbl.o 
	ld86 -d glbl.o -o glbl
boot.o : boot.asm
	as86 boot.asm -o boot.o
strt.o : strt.asm
	as86 strt.asm -o strt.o
glbl.o : glbl.asm
	as86 glbl.asm -o glbl.o
