;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;  µÚ19ÕÂ  Íâ¿Ç±àÐ´»ù´¡
;  ¶¼ÓÃÜÓë½âÃÜ£¨µÚËÄ°æ£©¡·
;  (c)  ¿´Ñ©Ñ§Ôº www.kanxue.com 2000-2018
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 
.586P
.MODEL FLAT,STDCALL
OPTION CASEMAP:NONE

include c:\masm32\include\windows.inc


PUBLIC				ShellStart0
PUBLIC				ShellEnd0
PUBLIC				ShellStart
PUBLIC				ShellEnd
PUBLIC				ImportTableBegin
PUBLIC				ImportTableEnd
PUBLIC				SHELL_DATA_0
PUBLIC				SHELL_DATA_1
PUBLIC				TlsTable
PUBLIC				RelocBaseBegin

assume 	fs:nothing

.code
;**********************************************************
ShellStart0 LABEL	DWORD
	pushad                    ; Íâ¿ÇÈë¿Úµã
	call	next0
;**********************************************************

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;                   ÒÔÏÂÊÇ×Ô¹¹ÔìµÄÍâ¿ÇµÄÊäÈë±í
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 
ImportTableBegin  LABEL	DWORD
ImportTable	 DD	AddressFirst-ImportTable  ;OriginalFirstThunk
		 DD	0,0                       ;TimeDataStamp,ForwardChain
AppImpRVA1	 DD	DllName-ImportTable       ;Name
AppImpRVA2	 DD	AddressFirst-ImportTable  ;FirstThunk
		 DD	0,0,0,0,0
AddressFirst	 DD	FirstFunc-ImportTable     ;Ö¸ÏòIMAGE_tHUNK_DATA
AddressSecond	 DD	SecondFunc-ImportTable    ;Ö¸ÏòIMAGE_tHUNK_DATA
AddressThird	 DD	ThirdFunc-ImportTable     ;Ö¸ÏòIMAGE_tHUNK_DATA
		 DD	0
DllName		 DB	'KERNEL32.dll'
		 DW	0
FirstFunc	 DW	0	
		 DB	'GetProcAddress',0
SecondFunc	 DW	0
		 DB	'GetModuleHandleA',0
ThirdFunc	 DW	0
		 DB	'LoadLibraryA',0
ImportTableEnd  LABEL	DWORD
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;               ÒÔÏÂÊÇ×Ô¹¹ÔìµÄ¼ÙÖØ¶¨Î»±í(´¦ÀíDLLÊ±ÓÃ£©
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 
RelocBaseBegin  LABEL	DWORD
RelocBase	 DD	0
		 DD	08h
		 DD	0
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;		ÒÔÏÂÊÇÐèÒªÓÉ¼Ó¿Ç³ÌÐòÐÞÕýµÄ±äÁ¿
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SHELL_DATA_0		LABEL	DWORD
ShellBase	 DD	0
ShellPackSize	 DD	0
TlsTable	 DB	18h dup (?)

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;               Íâ¿ÇÒýµ¼¶ÎÊ¹ÓÃµÄ±äÁ¿¿Õ¼ä
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Virtualalloc	 DB	'VirtualAlloc',0
VirtualallocADDR DD	0
imagebase	 DD	0
ShellStep	 DD	0 
ShellBase2	 DD	0 

;**********************************************************
next0:
	pop	ebp
	sub	ebp,(ImportTable-ShellStart0)

	;////////////////////////////////////////////////ÒÔÏÂ´úÂëÊÇ´¦ÀíDLLÊ±Æð×÷ÓÃ
	mov	eax, dword ptr [ebp+(ShellStep-ShellStart0)]
	.if	eax != 0;dllÎÄ¼þÍË³öÊ±×ßÕâÀï	
		push	ebp
		jmp	dword ptr [ebp+(ShellBase2-ShellStart0)]
	.endif
	inc	dword ptr [ebp+(ShellStep-ShellStart0)]
	
	mov	eax, dword ptr [esp+24h]
	mov	dword ptr [ebp+(imagebase-ShellStart0)], eax;È¡µ±Ç°Ó³Ïñ»ùÖ·£¬Èç¹ûÊÇEXEÔÚºóÃæ»áÓÃGetmulehandleÈ¡»ùÖ·µÄ
	;////////////////////////////////////////////////

	lea	esi,[ebp+(DllName-ShellStart0)]
	push	esi
	call	dword ptr [ebp+(AddressSecond-ShellStart0)]
	lea	esi,[ebp+(Virtualalloc-ShellStart0)]
	push	esi
	push	eax
	call	dword ptr [ebp+(AddressFirst-ShellStart0)]
	mov	dword ptr [ebp+(VirtualallocADDR-ShellStart0)],eax
	push	PAGE_READWRITE
	push	MEM_COMMIT
	push	dword ptr [ebp+(ShellPackSize-ShellStart0)]
	push	0
	call	dword ptr [ebp+(VirtualallocADDR-ShellStart0)]
	push	eax
	mov	dword ptr [ebp+(ShellBase2-ShellStart0)], eax;½«Íâ¿ÇµÚ¶þ¶ÎµØÖ··Åµ½ShellBase2£¬dllÍË³öÊ±»áÓÃµ½
	mov	ebx,dword ptr [ebp+(ShellBase-ShellStart0)]
	add	ebx,ebp
	push	eax
	push	ebx
	call	_aP_depack_asm
	pop	edx
	push	ebp
	jmp	edx

;*******Aplib½âÑ¹´úÂë********
_aP_depack_asm:
    pushad
    mov    esi, [esp + 36]    ; C calling convention
    mov    edi, [esp + 40]
    cld
    mov    dl, 80h
    xor    ebx, ebx
literal:
    movsb
    mov    bl, 2
nexttag:
    call   getbit
    jnc    literal

    xor    ecx, ecx
    call   getbit
    jnc    codepair
    xor    eax, eax
    call   getbit
    jnc    shortmatch
    mov    bl, 2
    inc    ecx
    mov    al, 10h
getmorebits:
    call   getbit
    adc    al, al
    jnc    getmorebits
    jnz    domatch
    stosb
    jmp    short nexttag
codepair:
    call   getgamma_no_ecx
    sub    ecx, ebx
    jnz    normalcodepair
    call   getgamma
    jmp    short domatch_lastpos
shortmatch:
    lodsb
    shr    eax, 1
    jz     donedepacking
    adc    ecx, ecx
    jmp    short domatch_with_2inc
normalcodepair:
    xchg   eax, ecx
    dec    eax
    shl    eax, 8
    lodsb
    call   getgamma
    cmp    eax, 32000
    jae    domatch_with_2inc
    cmp    ah, 5
    jae    domatch_with_inc
    cmp    eax, 7fh
    ja     domatch_new_lastpos
domatch_with_2inc:
    inc    ecx
domatch_with_inc:
    inc    ecx
domatch_new_lastpos:
    xchg   eax, ebp
domatch_lastpos:
    mov    eax, ebp
    mov    bl, 1
domatch:
    push   esi
    mov    esi, edi
    sub    esi, eax
    rep    movsb
    pop    esi
    jmp    short nexttag
getbit:
    add     dl, dl
    jnz     stillbitsleft
    mov     dl, [esi]
    inc     esi
    adc     dl, dl
stillbitsleft:
    ret
getgamma:
    xor    ecx, ecx
getgamma_no_ecx:
    inc    ecx
getgammaloop:
    call   getbit
    adc    ecx, ecx
    call   getbit
    jc     getgammaloop
    ret
donedepacking:
    sub    edi, [esp + 40]
    mov    [esp + 28], edi    ; return unpacked length in eax
    popad
    ret	8h

ShellEnd0  LABEL	DWORD


;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;               Íâ¿ÇµÚ¶þ²ã´úÂë
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
ShellStart LABEL	DWORD
	call	$+5
	pop	edx
	sub	edx,5h
	pop	ebp

	mov	eax, dword ptr [edx+(ShellStep_2-ShellStart)]
	.if	eax != 0;dllÍË³öÊ±´ÓÕâÀï½øÈëOEP	
	        popad
	        jmp ReturnOEP
	 .endif

	mov	ecx,3h
	lea	esi,[ebp+(AddressFirst-ShellStart0)]
	lea	edi,[edx+(GetprocaddressADDR-ShellStart)]
    MoveThreeFuncAddr:
	mov	eax,dword ptr [esi]
	mov	dword ptr [edi],eax
	add	esi,4h
	add	edi,4h
	loop	MoveThreeFuncAddr
	lea	eax,[ebp+(_aP_depack_asm-ShellStart0)]
	mov	dword ptr [edx+(aP_depackAddr-ShellStart)],eax
	mov	eax,dword ptr [ebp+(VirtualallocADDR-ShellStart0)]
	mov	dword ptr [edx+(S_VirtualallocADDR-ShellStart)],eax

	mov eax,[ebp+(imagebase-ShellStart0)]	;½«DLL»ùÖ·¶Á³ö
	mov	ebp,edx
	mov	dword ptr [ebp+(FileHandle-ShellStart)],eax
	mov	eax, dword ptr [ebp+(S_FileIsDll-ShellStart)]
	.if	eax == 0;Èç¹ûÊÇEXEÎÄ¼þ£¬ÔòÓÃ
		push	0
		call	dword ptr [ebp+(GetmulehandleADDR-ShellStart)]
		mov	dword ptr [ebp+(FileHandle-ShellStart)],eax	;È¡µÃµ±Ç°ÎÄ¼þ¾ä±ú
	.endif


	;*******È¡Ò»Ð©º¯ÊýÈë¿Ú
	lea	esi,dword ptr [ebp+(Ker32DllName-ShellStart)]
	push	esi
	call	dword ptr [ebp+(GetmulehandleADDR-ShellStart)]
	.if	eax==0
		push	esi
		call	dword ptr [ebp+(LoadlibraryADDR-ShellStart)]
	.endif
	mov	esi,eax
	lea	ebx,dword ptr [ebp+(S_Virtualfree-ShellStart)]
	push	ebx
	push	esi
	call	dword ptr [ebp+(GetprocaddressADDR-ShellStart)]
	mov	dword ptr [ebp+(S_VirtualfreeADDR-ShellStart)],eax
	;*******½âÑ¹Ëõ¸÷¶Î********
	mov	ebx,S_PackSection-ShellStart
    DePackNextSection:
	cmp	dword ptr [ebp+ebx],0h
	jz	AllSectionDePacked
	push	ebx
	push	PAGE_READWRITE
	push	MEM_COMMIT
	push	dword ptr [ebp+ebx]
	push	0
	call	dword ptr [ebp+(S_VirtualallocADDR-ShellStart)]	;ÉêÇëÄÚ´æ½øÐÐ¶ÁÐ´
	pop	ebx
	mov	esi,eax
	mov	eax,ebx
	add	eax,ebp
	mov	edi,dword ptr [eax+4h]
	add	edi,dword ptr [ebp+(FileHandle-ShellStart)]
	push	esi
	push	edi
	call	dword ptr [ebp+(aP_depackAddr-ShellStart)]
	mov	ecx,dword ptr [ebp+ebx]
	push	esi
	rep	movsb
	pop	esi
	push	ebx
	push	MEM_RELEASE
	push	0
	push	esi
	call	dword ptr [ebp+(S_VirtualfreeADDR-ShellStart)]	;ÊÍ·ÅÄÚ´æ
	pop	ebx
	add	ebx,0ch
	jmp	DePackNextSection
    AllSectionDePacked:
	;*******»Ö¸´Ô­ÊäÈë±í******
	mov	eax,dword ptr [ebp+(S_IsProtImpTable-ShellStart)]
	.if	eax == 0
		mov	edi,dword ptr [ebp+(ImpTableAddr-ShellStart)]
		add	edi,dword ptr [ebp+(FileHandle-ShellStart)]
	    GetNextDllFuncAddr:
		mov	esi,dword ptr [edi+0ch]
		.if	esi == 0
			jmp	AllDllFuncAddrGeted
		.endif
		add	esi,dword ptr [ebp+(FileHandle-ShellStart)]
		push	esi
		call	dword ptr [ebp+(GetmulehandleADDR-ShellStart)]
		.if	eax==0
			push	esi
			call	dword ptr [ebp+(LoadlibraryADDR-ShellStart)]
		.endif
		mov	esi,eax
		mov	edx,dword ptr [edi]
		.if	edx == 0
			mov	edx,dword ptr [edi+10h]
		.endif
		add	edx,dword ptr [ebp+(FileHandle-ShellStart)]
		mov	ebx,dword ptr [edi+10h]
		add	ebx,dword ptr [ebp+(FileHandle-ShellStart)]
	    GetNextFuncAddr:
		mov	eax,dword ptr [edx]
		.if	eax == 0
			jmp	AllFuncAddrGeted
		.endif
		push	ebx
		push	edx
		cdq
		.if	edx == 0	
			add	eax,2h
			add	eax,dword ptr [ebp+(FileHandle-ShellStart)]
		.else
			and	eax,7fffffffh
		.endif
		push	eax
		push	esi
		call	dword ptr [ebp+(GetprocaddressADDR-ShellStart)]
		mov	dword ptr [ebx],eax
		pop	edx
		pop	ebx
		add	edx,4h
		add	ebx,4h
		jmp	GetNextFuncAddr
	    AllFuncAddrGeted:
		add	edi,14h
		jmp	GetNextDllFuncAddr
	    AllDllFuncAddrGeted:
	.else
		mov	edx,dword ptr [ebp+(ImpTableAddr-ShellStart)]
		add	edx,ebp
	    GetNextDllFuncAddr2:
		mov	edi,dword ptr [edx]
		.if	edi == 0
			jmp	AllDllFuncAddrGeted2
		.endif
		add	edi,dword ptr [ebp+(FileHandle-ShellStart)]
		add	edx,5h
		mov	esi,edx
		push	esi
		call	dword ptr [ebp+(GetmulehandleADDR-ShellStart)]
		.if	eax==0
			push	esi
			call	dword ptr [ebp+(LoadlibraryADDR-ShellStart)]
		.endif
		movzx	ecx,byte ptr [esi-1]
		add	esi,ecx
		mov	edx,esi
		mov	esi,eax
		inc	edx
		mov	ecx,dword ptr [edx]
		add	edx,4h
	    GetNextFuncAddr2:
		push	ecx
		movzx	eax,byte ptr [edx]
		.if	eax == 0
			inc	edx
			push	edx
			mov	eax,dword ptr [edx]
			push	eax
			push	esi
			call	dword ptr [ebp+(GetprocaddressADDR-ShellStart)]
			mov	dword ptr [edi],eax
			pop	edx
			add	edx,4h
		.else
			inc	edx
			push	edx
			push	edx
			push	esi
			call	dword ptr [ebp+(GetprocaddressADDR-ShellStart)]
			mov	dword ptr [edi],eax
			pop	edx
			movzx	eax,byte ptr [edx-1]
			add	edx,eax
		.endif
		inc	edx
		add	edi,4h
		pop	ecx
		loop	GetNextFuncAddr2
		jmp	GetNextDllFuncAddr2
	    AllDllFuncAddrGeted2:
	.endif

	;*******ÐÞÕýÖØ¶¨Î»Êý¾Ý

	mov	esi, dword ptr [ebp+ (S_RelocADDR-ShellStart)]
	.if	esi != 0
		add	esi, dword ptr [ebp+(FileHandle-ShellStart)]
		mov	edi, dword ptr [ebp+(FileHandle-ShellStart)]
		mov	ebx, edi
		sub	edi, dword ptr [ebp+(S_PeImageBase-ShellStart)]
		movzx	eax, byte ptr [esi]
		.while	al
			.if al == 3h
				inc	esi
				add	ebx, dword ptr [esi]
				add	dword ptr [ebx], edi
				add	esi, 4h
			.else
				inc	esi
				add	ebx, eax
				add	dword ptr [ebx], edi
			.endif
			movzx	eax, byte ptr [esi]
		.endw
	.endif

	;*******anti  dump*****************
	push	fs:[30h]
	pop	eax
	test	eax,eax
	js	fuapfdw_is9x  
fuapfdw_isNT:
	mov	eax, [eax+0ch]
	mov	eax, [eax+0ch]
	mov	dword ptr [eax+20h], 1000h 
	jmp	fuapfdw_finished
fuapfdw_is9x:
	push	0
	call	dword ptr [ebp+(GetmulehandleADDR-ShellStart)]
	test	edx, edx
	jns	fuapfdw_finished 
	cmp	dword ptr [edx+8h], -1
	jne	fuapfdw_finished  
	mov	edx, [edx+4]  
	mov	dword ptr [edx+50h], 1000h 
fuapfdw_finished:

	inc     dword ptr [ebp+(ShellStep_2-ShellStart)] ;dll
	;*************×¼±¸·µ»ØOEP***************
	mov	eax,dword ptr [ebp+(OEP-ShellStart)]
	add	eax,dword ptr [ebp+(FileHandle-ShellStart)]
	add	dword ptr [ebp+(ReturnOEP-ShellStart)+1],eax ;eax=OEP
	popad
ReturnOEP:
	push dword ptr[0]
	ret

;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;		ÒÔÏÂÊÇÐèÒªÓÉ¼Ó¿Ç³ÌÐòÐÞÕýµÄ±äÁ¿
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SHELL_DATA_1		LABEL	DWORD
OEP			DD	0
S_IsProtImpTable	DD	0
ImpTableAddr		DD	0
S_FileIsDll		DD	0
S_RelocADDR		DD	0  ;Ô­Ê¼ÖØ¶¨Î»µØÖ·£¨ÐÞÕýµÄÖØ¶¨Êý¾Ý»¹·ÅÔÚ´Ë£©
S_PeImageBase           DD	0  ;Ô­Ê¼Ó³Ïó»ùÖ·
S_PackSection		DB	0a0h dup (?)


;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
;		ÒÔÏÂÊÇÍâ¿ÇµÚ¶þ¶ÎÊ¹ÓÃµÄ±äÁ¿
;^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

GetprocaddressADDR	DD	0
GetmulehandleADDR	DD	0
LoadlibraryADDR		DD	0
S_VirtualallocADDR	DD	0
FileHandle		DD	0
aP_depackAddr		DD	0
ShellStep_2             DD	0
S_VirtualfreeADDR	DD	0
Ker32DllName		DB	'KERNEL32.dll',0
S_Virtualfree		DB	'VirtualFree',0

ShellEnd LABEL		DWORD


end