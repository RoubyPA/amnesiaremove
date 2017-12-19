# Amnesia remove project

all: arm

arm: arm.o error.o random.o remove.o
	gcc -Wall -std=c99 arm.o error.o random.o remove.o -o arm

arm.o: arm.c
	gcc -Wall -std=c99 -c arm.c -o arm.o

random.o: random.c
	gcc -Wall -std=c99 -c random.c -o random.o

error.o: error.c
	gcc -Wall -std=c99 -c error.c -o error.o

remove.o: remove.c
	gcc -Wall -std=c99 -c remove.c -o remove.o

install: arm
	mkdir -p $(PREFIX)/bin
	install ./arm $(PREFIX)/bin/

clean:
	rm -v *.o
