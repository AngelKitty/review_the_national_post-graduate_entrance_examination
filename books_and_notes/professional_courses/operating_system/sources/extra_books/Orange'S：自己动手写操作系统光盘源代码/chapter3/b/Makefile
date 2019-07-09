##################################################
# Makefile of pmtestx.asm (x=[1,2,3...])
##################################################

SRC:=pmtest2.asm
BIN:=$(subst .asm,.com,$(SRC))

.PHONY : everything

everything : $(BIN)
	sudo mount -o loop pm.img /mnt/floppy/
	sudo cp $(BIN) /mnt/floppy/ -v
	sudo umount /mnt/floppy/

$(BIN) : $(SRC)
	nasm $< -o $@

