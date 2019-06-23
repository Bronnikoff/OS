binary: prog2.o libworld.so
	gcc -o binary prog2.o -L. -lworld -Wl,-rpath,.
	rm -f *.o

prog2.o: prog2.c
	gcc -c prog2.c

libworld.so: Massive.o
	gcc -shared -o libworld.so Massive.o

Massive.o: Massive.c
	gcc -c -fPIC Massive.c

clean:
	rm -f *.o *.so *.out binary
