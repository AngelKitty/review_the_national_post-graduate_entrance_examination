DATA    SEGMENT
M       DW  12
N       DW  8
RESULT  DW  ?
DATA    ENDS
STACK   SEGMENT  STACK
        DW  200 DUP (?)
STACK   ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,SS:STACK,DS:DATA
START:  MOV  AX,DATA
        MOV  DS,AX
        MOV  AX,M    
        MOV  BX,N   
        CALL  FAR PTR SUBP
        MOV  RESULT,CX  
        MOV  AH,4CH
        INT  21H


SUBP    PROC  FAR
        PUSH  AX        
        PUSH  BX
        PUSH  DX
        CMP  AX,BX     
        JZ  NEXT     
        JA  GREAT   
        XCHG  AX,BX  
GREAT:  XOR  DX,DX
        DIV  BX          
        AND  DX,DX   
        JZ  NEXT   
        MOV  AX,BX 
        MOV  BX,DX  
        JMP  GREAT  
NEXT:   MOV  CX,BX 
        POP  DX
        POP  BX
        POP  AX
        RET
SUBP    ENDP
CODE    ENDS
        END  START