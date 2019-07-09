#######################
# Makefile for foobar #
#######################

# Programs, flags, etc.
ASM		= nasm
CC		= gcc
LD		= ld
ASMFLAGS	= -f elf
CFLAGS		= -c
LDFLAGS		= -s

# This Program
BIN		= foobar
OBJS		= foo.o bar.o

# All Phony Targets
.PHONY : everything final image clean realclean disasm all buildimg

# Default starting position
everything : $(BIN)

all : realclean everything

final : all clean

clean :
	rm -f $(OBJS)

realclean :
	rm -f $(OBJS) $(BIN)

$(BIN) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(BIN) $(OBJS)

foo.o : foo.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

bar.o: bar.c
	$(CC) $(CFLAGS) -o $@ $<
