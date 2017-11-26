# Amnesia remove project
all: arm

arm: arm.o
	gcc -Wall -std=c99 arm.o -o arm

arm.o: arm.c
	gcc -Wall -std=c99 -c arm.c -o arm.o

# TODO install

clean:
	rm -v *.o
