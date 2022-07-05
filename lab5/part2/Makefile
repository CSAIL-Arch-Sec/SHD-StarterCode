CC := gcc
AS := as
LD := ld

TARGET := a b

DEPS := ../inc/lab5.h Makefile

ASFLAGS :=
CFLAGS := -I../inc -g -O0 -fno-stack-protector

.PHONY: all clean

all: $(TARGET)

clean:
	rm -rf build $(TARGET)

a: build/part2A.o build/mainA.o
	@echo " LD    $<"
	@mkdir -p build
	@$(CC) -o $@ build/mainA.o build/part2A.o

build/part2A.o: part2A.c $(DEPS) build/mainA.o
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $< $(CFLAGS) -o $@

b: build/part2B.o build/mainB.o build/gadgets.o
	@echo " LD    $<"
	@mkdir -p build
	@$(CC) -o $@ build/mainB.o build/part2B.o build/gadgets.o

build/part2B.o: part2B.c $(DEPS) build/mainB.o
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $< $(CFLAGS) -o $@

build/mainA.o: main.c Makefile ../inc
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $< $(CFLAGS) -o $@

build/mainB.o: main.c Makefile ../inc
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $< $(CFLAGS) -D PART2B -o $@

build/gadgets.o: gadgets.s
	@echo " AS    $<"
	@mkdir -p build
	@$(AS) -c $< -o $@
