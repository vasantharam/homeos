include makefile.inc

all : bootblk
bootblk : arch/i386/i386.so cmain
	cat arch/i386/boot arch/i386/strt arch/i386/glbl cmain  > bootblk; dd if=/dev/zero of=out count=128 bs=4096; cat out >> bootblk
	
example/cmain.so : example/cmain.c example/cmain.h 
	make -C example
arch/i386/i386.so: arch/i386/*.c arch/i386/*.asm arch/i386/internal-include/*.h arch/i386/interface-include/*.h
	make -C arch/i386 all
kernel/kernel.so : kernel/*.c  kernel/include/*.h
	make -C kernel all
cmain : example/cmain.so arch/i386/i386.so kernel/kernel.so
	ld -o .cmain -Ttext 0x10400 -T linker_script.x86.lds example/*.o kernel/*.o arch/i386/*.o; objcopy -O binary -S .cmain cmain; objdump -D .cmain > .cmain_disassembly; objdump -b binary -m i386 -D cmain > cmain_disassembly
.PHONY : clean
clean : 
	rm -Rf *.o cmain bootblk; make -C arch/i386 clean; make -C kernel clean

