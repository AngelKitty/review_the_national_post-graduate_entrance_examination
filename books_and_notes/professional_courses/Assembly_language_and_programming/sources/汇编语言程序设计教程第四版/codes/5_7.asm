DATA    SEGMENT
N       DW  12
X       DW  55
DATBUF  DW  4,36,50,55,69,87,88,2AH,100H,510,1234,2000H,?
DATA    ENDS
STACK   SEGMENT  STACK
        DW  100 DUP (?)
STACK   ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,SS:STACK,DS:DATA,ES:DATA
START:  PUSH  ES
        MOV  AX,DATA
        MOV  DS,AX
        PUSH  DS
        POP  ES
        MOV  AX,X  
        MOV  DX,N 
        MOV  CX,N 
        MOV  DI,OFFSET DATBUF 
        CLD
REPNE   SCASW  
        JE  DELETE
        DEC  DX
        MOV  SI,DX
        ADD  SI,DX
A1:     CMP  AX,DATBUF[SI]  
        JL  A2
        MOV  DATBUF[SI+2],AX
        JMP  A3
A2:     MOV  BX,DATBUF[SI]
        MOV  DATBUF[SI+2],BX
        SUB  SI,2
        JMP  A1
A3:     ADD  DX,2     
        JMP  A4
DELETE: JCXZ  NEXT
LOOPT:  MOV  BX,[DI] 
        MOV  [DI-2],BX
        ADD  DI,2
        LOOP  LOOPT
NEXT:   DEC  DX   
A4:     POP  ES
        MOV  AH,4CH
        INT  21H
CODE    ENDS
        END  START
