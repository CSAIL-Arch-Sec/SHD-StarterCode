CC := gcc
AS := as
LD := ld

TARGET := a b c

DEPS := ../inc/lab5.h Makefile

OBJECTS := main.o
BUILD_OBJECTS := $(patsubst %,build/%,$(OBJECTS))

ASFLAGS :=
CFLAGS := -I../inc -g -O0 -fno-stack-protector

.PHONY: all clean

all: $(TARGET)

clean:
	rm -rf build $(TARGET)

a: build/part1A.o build/main.o
	@echo " LD    $<"
	@mkdir -p build
	@$(CC) -o $@ $(BUILD_OBJECTS) build/part1A.o

build/part1A.o: part1A.c $(DEPS) $(BUILD_OBJECTS)
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $< $(CFLAGS) -o $@

b: build/part1B.o build/main.o
	@echo " LD    $<"
	@mkdir -p build
	@$(CC) -o $@ $(BUILD_OBJECTS) build/part1B.o

build/part1B.o: part1B.c $(DEPS) $(BUILD_OBJECTS)
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $< $(CFLAGS) -o $@

c: build/part1C.o build/main.o
	@echo " LD    $<"
	@mkdir -p build
	@$(CC) -o $@ $(BUILD_OBJECTS) build/part1C.o

build/part1C.o: part1C.c $(DEPS) $(BUILD_OBJECTS)
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $< $(CFLAGS) -o $@

build/main.o: main.c Makefile ../inc
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $< $(CFLAGS) -o $@
