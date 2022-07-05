include ../cpu.mk

TARGETS=attacker
UTILS=util.o 

all: $(TARGETS)

$(UTILS): %.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

%.o: %.c util.h
	$(CC) $(CFLAGS)  -c $< 

$(TARGETS): %:%.o util.o
	$(CC) $(CFLAGS) $^ -o $@

run_victim-2:
	taskset -c $(SENDER_CPU) ./victim-2

run_victim-3:
	taskset -c $(SENDER_CPU) ./victim-3

run_victim-4:
	taskset -c $(SENDER_CPU) ./victim-4

run_attacker: attacker
	taskset -c $(RECEIVER_CPU) ./attacker

.PHONY:	clean

clean:
	$(RM) *.o $(HELPERS) $(TARGETS) 
