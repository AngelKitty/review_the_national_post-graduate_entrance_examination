##################################################
# Makefile
##################################################

BOOT:=boot.asm
LDR:=loader.asm
BOOT_BIN:=$(subst .asm,.bin,$(BOOT))
LDR_BIN:=$(subst .asm,.bin,$(LDR))

.PHONY : everything

everything : $(BOOT_BIN) $(LDR_BIN)
	dd if=$(BOOT_BIN) of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp $(LDR_BIN) /mnt/floppy/ -v
	sudo umount /mnt/floppy/

clean :
	rm -f $(BOOT_BIN) $(LDR_BIN)

$(BOOT_BIN) : $(BOOT)
	nasm $< -o $@

$(LDR_BIN) : $(LDR)
	nasm $< -o $@

