# Amnesia remove project

all: arm

arm: arm.o
	gcc -Wall -std=c99 arm.o -o arm

arm.o: arm.c
	gcc -Wall -std=c99 -c arm.c -o arm.o

install: arm
	mkdir -p $(PREFIX)/bin
	install ./arm $(PREFIX)/bin/

clean:
	rm -v *.o
