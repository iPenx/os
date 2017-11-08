BUILD_DIR=build
ENTRY_POINT=0xc0001500

AS=nasm
CC=gcc
LD=ld

LIB=-I lib/
INCLUDE=-I include/

FLAGS_AS=-f elf
FLAGS_C=$(LIB) $(INCLUDE) -m32 -c -W -fno-builtin -fno-stack-protector -Wstrict-prototypes -Wmissing-prototypes
FLAGS_LD=-m elf_i386 -Ttext $(ENTRY_POINT) -e main -Map $(BUILD_DIR)/kernel.map

OBJS=$(BUILD_DIR)/kernel.o $(BUILD_DIR)/init.o $(BUILD_DIR)/memory.o $(BUILD_DIR)/thread.o $(BUILD_DIR)/list.o $(BUILD_DIR)/interrupt.o $(BUILD_DIR)/timer.o $(BUILD_DIR)/print.o $(BUILD_DIR)/debug.o $(BUILD_DIR)/bitmap.o $(BUILD_DIR)/string.o $(BUILD_DIR)/switch_to.o $(BUILD_DIR)/intr_proc.o

##### make #####
xxxxx:

##########     bootloader     ##########
mbr: include/boot.inc boot/mbr.asm
	nasm boot/mbr.asm -o $(BUILD_DIR)/mbr.bin -I include/

install_mbr: build/mbr.bin
	dd if=$(BUILD_DIR)/mbr.bin of=c.img conv=notrunc

loader: include/boot.inc boot/loader.asm
	nasm boot/loader.asm -o $(BUILD_DIR)/loader.bin -I include/

install_loader: build/loader.bin
	dd if=$(BUILD_DIR)/loader.bin of=c.img seek=2 conv=notrunc

##########     kernel     ###########
$(BUILD_DIR)/kernel.o: kernel/kernel.c include/print.h include/stdint.h include/init.h
	$(CC) $(FLAGS_C) $< -o $@

$(BUILD_DIR)/init.o: kernel/init.c include/init.h include/memory.h include/print.h include/stdint.h include/interrupt.h include/timer.h
	$(CC) $(FLAGS_C) $< -o $@

$(BUILD_DIR)/memory.o: kernel/memory.c include/memory.h include/global.h include/x86.h include/process.h include/bitmap.h include/stdint.h include/string.h include/print.h
	$(CC) $(FLAGS_C) $< -o $@

#$(BUILD_DIR)/process.o: kernel/process.c include/process.h include/global.h include/memory.h include/list.h include/thread.h include/debug.h
#	$(CC) $(FLAGS_C) $< -o $@

$(BUILD_DIR)/thread.o: kernel/thread.c include/thread.h include/process.h include/global.h include/list.h include/debug.h
	$(CC) $(FLAGS_C) $< -o $@

$(BUILD_DIR)/interrupt.o: kernel/interrupt.c include/interrupt.h include/stdint.h include/global.h include/io.h include/print.h
	$(CC) $(FLAGS_C) $< -o $@

$(BUILD_DIR)/timer.o: device/timer.c include/timer.h include/stdint.h include/io.h include/print.h
	$(CC) $(FLAGS_C) $< -o $@

$(BUILD_DIR)/debug.o: lib/debug.c include/debug.h
	$(CC) $(FLAGS_C) $< -o $@

$(BUILD_DIR)/bitmap.o: lib/bitmap.c include/stdint.h include/bitmap.h
	$(CC) $(FLAGS_C) $< -o $@

$(BUILD_DIR)/list.o: lib/list.c include/list.h include/stdint.h include/debug.h
	$(CC) $(FLAGS_C) $< -o $@

$(BUILD_DIR)/string.o: lib/string.c include/string.h include/stdint.h include/debug.h
	$(CC) $(FLAGS_C) $< -o $@

$(BUILD_DIR)/intr_proc.o: kernel/intr_proc.asm
	$(AS) $(FLAGS_AS) $< -o $@

$(BUILD_DIR)/print.o: lib/print.asm
	$(AS) $(FLAGS_AS) $< -o $@

$(BUILD_DIR)/switch_to.o: kernel/thread_c_switch_to.asm
	$(AS) $(FLAGS_AS) $< -o $@

##########     Link     ##########
$(BUILD_DIR)/kernel.bin: $(OBJS)
	$(LD) $(FLAGS_LD) $^ -o $@

.PHONY: mkdir_build install_kernel clean all run
mkdir_build:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi

install_kernel:
	dd if=$(BUILD_DIR)/kernel.bin of=c.img count=200 seek=9 conv=notrunc

clean: 
	rm -f $(BUILD_DIR)/*

build: $(BUILD_DIR)/kernel.bin

all: mkdir_build build install_kernel 

run: all
	bochs
