DATA    SEGMENT
A       DB  46
X       DB  5 DUP (?) 
B       DB  100
Y       DB  3 DUP (?)
DATA    ENDS
STACK   SEGMENT  STACK
        DW  100 DUP (?)
STACK   ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,SS:STACK,DS:DATA
START:  MOV  AX,DATA
        MOV  DS,AX
        MOV  CX,8
        MOV  DL,10110101B 
        MOV  AH,A      
        SUB  AH,2
        MOV  DH,B
        ADD  DH,2
        LEA  BX,X
        LEA  SI,Y
A1:     SAL  DL,1   
        JNC  A2
        ADD  AH,2   
        MOV  AL,AH
        ADD  AL,23
        MOV  [BX],AL  
        INC  BX
        JMP  A3
A2:     SUB  DH,2   
        MOV  AL,DH
        ADD  AL,34
        MOV  [SI],AL  
        INC  SI
A3:     LOOP  A1
        MOV  AH,4CH
        INT  21H
CODE    ENDS
        END  START
