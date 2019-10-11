DATA		SEGMENT
STRING		DB  00,01,02,03,04,05,06,07,08,09
                DB  0AH,0BH,0CH,0DH,0EH,0FH
SOUR		EQU  STRING
DEST		EQU  STRING+10
LEN  		EQU  16
DATA		ENDS
STACK		SEGMENT
		DW 50 DUP(?)
STACK		ENDS
CODE		SEGMENT
 		ASSUME  CS:CODE,DS:DATA,SS:STACK
START           PROC  FAR
 	        PUSH  DS 
		MOV  AX,0
 		PUSH  AX
   		MOV  AX,DATA
		MOV  DS,AX
		MOV  ES,AX
 		MOV  CX,LEN
       		MOV  SI,OFFSET SOUR
 	        MOV  DI,OFFSET DEST
       	 	CLD     				
                PUSH  SI
 		ADD  SI,LEN-1
 		CMP  SI,DI
 		POP  SI
 		JL MVE
		STD			
		ADD  SI,LEN-1
	   	ADD  DI,LEN-1
MVE:    	REP  MOVSB			
                RET
START           ENDP
CODE		ENDS
		END START
