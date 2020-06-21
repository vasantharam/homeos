include ../../makefile.inc
.PHONY: all
all: arm.so bootblk
.PHONY: clean
clean : 
	rm -Rf *.o; rm -Rf *.so 

arm.so: *.S *.c
	$(CC) -DL_dvmd_lnx -DL_udivsi3 -DL_umodsi3 -Darm -shared -I"internal-include" -I"interface-include" -I../../ -I../../include *.c start.S -o arm.so; $(CC) -DL_dvmd_lnx -DL_udivsi3 -DL_umodsi3 -c -Darm -I"internal-include" -I"interface-include" -I../../ -I../../include *.c start.S; rm -Rf boot.o strt.o glbl.o
umod.o: lib1funcs.S
	$(CC) -c -DL_dvmd_lnx -DL_umodsi3 lib1funcs.S; mv lib1funcs.o umod.o

udiv.o: lib1funcs.S
	$(CC) -c -DL_udivsi3 lib1funcs.S; mv lib1funcs.o udiv.o

bootblk: arm.so umod.o udiv.o
	arm-linux-ld -o .cmain -Ttext 0x10000 -T../../linker_script.arm.lds *.o; arm-linux-objcopy -O binary -S .cmain cmain; arm-linux-objdump -D .cmain > .cmain_disassembly; arm-linux-objdump -b binary -m arm -D cmain > cmain_disassembly; cp cmain bootblk
