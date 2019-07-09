##################################################
# Makefile of pmtestx.asm (x=[1,2,3...])
##################################################

SRC:=boot.asm
BIN:=$(subst .asm,.bin,$(SRC))

.PHONY : everything

everything : $(BIN)
	dd if=$(BIN) of=a.img bs=512 count=1 conv=notrunc

$(BIN) : $(SRC)
	nasm $< -o $@

