include ../makefile.inc
INCLUDE = -I../arch -I.. -Iinclude -I../include
.PHONY : all
all : kernel.so

.PHONY : clean
clean : 
	rm -Rf *.o; rm -Rf *.so
kernel.so : *.c
	$(CC) -shared -Di386  $(INCLUDE) *.c -o kernel.so; $(CC) -c -Di386  $(INCLUDE) *.c 
