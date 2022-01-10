# Grace Biemans (geb965) & Nick Heleta (nwh397)

.PHONY: clean


libMem.a: init.o alloc.o free.o display.o
	ar -rcs libMem.a init.o alloc.o free.o display.o



init.o: init.c init.h alloc.h
	gcc -Wall -Wextra -c init.c

alloc.o: alloc.c init.h alloc.h
	gcc -Wall -Wextra -c alloc.c

free.o: free.c init.h free.h
	gcc -Wall -Wextra -c free.c

display.o: display.c init.h display.h
	gcc -Wall -Wextra -c display.c


clean:
	rm -f *.o libMem.a
