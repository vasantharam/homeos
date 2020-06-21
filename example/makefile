include ../makefile.inc
INCLUDE = -I.. -I../include
.PHONY : all
all : cmain.so

.PHONY : clean
clean : 
	rm -Rf *.o; rm -Rf *.so
cmain.so : *.c *.h
	$(CC) -shared -Di386  $(INCLUDE) *.c -o cmain.so; $(CC) -c -Di386  $(INCLUDE) *.c 
