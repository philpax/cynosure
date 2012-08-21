.PHONY: all clean

all: bootloader.bin call_test.bin loop_test.bin rebuild_test.bin

%.bin: %.asm
	nasm -O0 -f bin -o $@ $<

clean:
	rm -f *.bin