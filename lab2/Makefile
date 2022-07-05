CC := gcc
AS := as
LD := ld

OBJECTS_COMMON := main.o lab2_helper.o

OBJECTS_PART1 := $(OBJECTS_COMMON) attacker-part1.o
TARGET_PART1  := part1

OBJECTS_PART2 := $(OBJECTS_COMMON) attacker-part2.o
TARGET_PART2  := part2

OBJECTS_PART3 := $(OBJECTS_COMMON) attacker-part3.o
TARGET_PART3  := part3

BUILD_OBJECTS_PART1 := $(patsubst %,build/%,$(OBJECTS_PART1))
BUILD_OBJECTS_PART2 := $(patsubst %,build/%,$(OBJECTS_PART2))
BUILD_OBJECTS_PART3 := $(patsubst %,build/%,$(OBJECTS_PART3))

TARGETS := $(TARGET_PART1) $(TARGET_PART2) $(TARGET_PART3)

ASFLAGS :=
CFLAGS := -Iinc -g -O0

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -rf build $(TARGETS)

build/%.o: src-common/%.s
	@echo " AS    $<"
	@mkdir -p build
	@$(AS) -c $(ASFLAGS) $< -o $@

build/%.o: src-common/%.c
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $(CFLAGS) $< -o $@

build/%.o: part1-src/%.c
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $(CFLAGS) $< -o $@

build/%.o: part2-src/%.c
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $(CFLAGS) $< -o $@

build/%.o: part3-src/%.c
	@echo " CC    $<"
	@mkdir -p build
	@$(CC) -c $(CFLAGS) $< -o $@

$(TARGET_PART1): $(BUILD_OBJECTS_PART1) Makefile
	@echo " LD    $@"
	@mkdir -p build
	@$(CC) -o $@ $(BUILD_OBJECTS_PART1)

$(TARGET_PART2): $(BUILD_OBJECTS_PART2) Makefile
	@echo " LD    $@"
	@mkdir -p build
	@$(CC) -o $@ $(BUILD_OBJECTS_PART2)

$(TARGET_PART3): $(BUILD_OBJECTS_PART3) Makefile
	@echo " LD    $@"
	@mkdir -p build
	@$(CC) -o $@ $(BUILD_OBJECTS_PART3)