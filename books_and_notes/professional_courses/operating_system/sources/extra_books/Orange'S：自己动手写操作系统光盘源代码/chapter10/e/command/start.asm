;; start.asm

extern	main
extern	exit

bits 32

[section .text]

global _start

_start:
	push	eax
	push	ecx
	call	main
	;; need not clean up the stack here

	push	eax
	call	exit

	hlt	; should never arrive here

