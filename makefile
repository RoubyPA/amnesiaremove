# Amnesia remove project

all: build

build: src/arm.makefile
	make -C src -f arm.makefile

install: src/arm
	mkdir -p $(PREFIX)/bin
	install ./src/arm $(PREFIX)/bin/

clean:
	make -C src -f arm.makefile clean
