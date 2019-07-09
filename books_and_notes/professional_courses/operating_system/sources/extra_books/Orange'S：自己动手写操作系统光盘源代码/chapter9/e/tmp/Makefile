# Makefile
CC	= gcc
CFLAGS	= -g
SRC	= f.c
BIN	= f

# All Phony Targets
.PHONY : everything final clean realclean all

# Default starting position
everything : $(BIN)

all : realclean everything

final : all clean

clean :
	rm -f *.o

realclean :
	rm -f $(BIN) *.o

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

