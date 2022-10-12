CC := gcc
AS := as
LD := ld
OBJCOPY := objcopy

TARGET := part3 gadgets.bin gadgets.o

DEPS := ../inc/lab5.h Makefile

ASFLAGS :=
CFLAGS := -I../inc -g -O0 -fno-stack-protector

.PHONY: all clean

all: $(TARGET)

clean:
	rm -rf build $(TARGET)

part3: build/part3.o build/main.o
	@echo " LD    $<"
	@mkdir -p build
	@$(CC) -o $@ build/main.o build/part3.o

build/part3.o: part3.c $(DEPS)
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $< $(CFLAGS) -o $@

build/main.o: main.c Makefile ../inc gadgets.bin
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $< $(CFLAGS) -o $@

gadgets.o: ../part2/gadgets.s
	@echo " AS    $<"
	@mkdir -p build
	@$(AS) -c $< -o $@

gadgets.bin: gadgets.o
	@echo " OBJC  $<"
	@mkdir -p build
	@$(OBJCOPY) -O binary gadgets.o gadgets.bin
