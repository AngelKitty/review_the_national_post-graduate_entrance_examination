DATA  	 SEGMENT
BUF      DW  -88,12,-20,-55,40,11,0,64,44,66
COUNT    EQU 10
DATA     ENDS
STACK    SEGMENT  STACK
         DW  128  DUP(0)
STACK	 ENDS
CODE     SEGMENT
         ASSUME   CS:CODE,DS:DATA,SS:STACK
MAIN     PROC   FAR
START:   PUSH   DS
         SUB    AX,AX
         PUSH   AX
         MOV    AX,DATA
         MOV    DS,AX
         MOV    DI,COUNT-1
         MOV    BX,0
CYCL1:   MOV    AX,BUF[BX] 
         MOV    DX,BX
         MOV    CX,BX	
         SHR    CX,1	
         MOV    SI,BX
CYCL2:   INC    CX   
         ADD    SI,2
         CMP    AX,BUF[SI] 
         JLE    CONTI
         MOV   DX,SI      
         MOV   AX,BUF[SI]		
CONTI:   CMP   CX,COUNT-1	
	 JL    CYCL2		
         XCHG  AX,BUF[BX]       
         MOV   SI,DX
         MOV   BUF[SI],AX
         INC    BX			
         INC    BX
         DEC    DI		
         JNE    CYCL1	
         RET
MAIN     ENDP
CODE     ENDS
         END   START
