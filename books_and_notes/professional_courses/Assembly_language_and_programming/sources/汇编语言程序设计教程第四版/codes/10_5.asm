DATA	SEGMENT
AIRPLN  DB  3EH,04,0C4H,04
DATA 	ENDS
CLEAR	MACRO			
 	MOV   AH,6		
 	MOV   AL,0	
 	MOV   CH,0		
 	MOV   CL,0
 	MOV   DH,24		
 	MOV   DL,79
        MOV   BH,07		
 	INT   10H
 	ENDM
CODE 	SEGMENT
 	ASSUME  CS:CODE,DS:DATA
MAIN	PROC  FAR
START:  PUSH  DS
 	MOV   AX,0
 	PUSH  AX
        MOV   AX,DATA
 	MOV   DS,AX
        CLEAR             
        MOV   AH,0FH	  
        INT   10H
        MOV   DH,15	
 	MOV   DL,0
BEGIN:	MOV   SI,2
    	MOV   CX,1
 	LEA    DI,AIRPLN
DISP:	CMP    DL,80
 	JAE     EXIT		
        MOV    AH,2	
        INT     10H
        MOV    AH,9	
 	MOV    AL,[DI]
 	MOV    BL,[DI+1]
        INT    10H
 	CALL   DELAY	
        MOV   AH,9
        MOV   AL,' '	  
        MOV   BL,04
        INT   10H
 	INC   DL
 	ADD   DI,2
 	DEC   SI
 	JNZ   DISP
 	JMP   BEGIN
EXIT:	RET
MAIN	ENDP
DELAY	PROC   NEAR	
 	PUSH   CX
 	PUSH   DX
        MOV   DX,500
DY1:	MOV   CX,2801	
DY2:	LOOP   DY2
 	DEC    DX
 	JNZ    DY1
 	POP    DX
 	POP    CX
 	RET
DELAY	ENDP
CODE	ENDS
	END    START
