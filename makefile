# Amnesia remove project

all: build

build: src/arm.makefile
	make -C src -f arm.makefile

install: arm
	mkdir -p $(PREFIX)/bin
	install ./arm $(PREFIX)/bin/

clean:
	make -C src -f arm.makefile clean
