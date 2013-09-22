.PHONY: all clean finedb lib src test

finedb:
	cd lib; make
	cd src; make

all:
	cd lib; make all
	cd src; make all

clean:
	cd lib; make clean
	cd src; make clean

lib:
	cd lib; make

src:
	cd src; make

