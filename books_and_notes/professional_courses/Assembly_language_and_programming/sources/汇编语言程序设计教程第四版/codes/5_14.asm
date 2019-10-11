STACK   SEGMENT  STACK
        DW  128 DUP(?)
STACK   ENDS
DATA    SEGMENT
SUM     DB  4 DUP(?)
DATA    ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,SS:STACK,DS:DATA
START:  MOV  AX,DATA
        MOV  DS,AX
        MOV  BX,1 
        MOV  AX,0  
        MOV  CX,100  
NEXT:   ADD  AX,BX
        INC  BX   
        LOOP  NEXT
        MOV   DL,0
LP1:    CMP  AX,3E8H  
        JB  LP2    
        INC  DL  
        SUB  AX,3E8H
        JMP  LP1
LP2:    MOV  SUM+3,DL  
        MOV  DL,0
LP3:    CMP  AX,64H  
        JB  LP4    
        INC  DL   
        SUB  AX,64H
        JMP  LP3
LP4:    MOV  SUM+2,DL  
        MOV  DL,0
LP5:    CMP  AX,0AH  
        JB  LP6     
        INC  DL   
        SUB  AX,0AH
        JMP  LP5
LP6:    MOV  SUM+1,DL  
        MOV  SUM,AL   
        MOV  AH,4CH
        INT  21H
CODE    ENDS
        END  START
