.PHONY: all machinecode images clean

all: machinecode images

images:
	make -C Debug

machinecode:
	make -C machinecode

clean:
	make -C Debug clean
	make -C machinecode clean