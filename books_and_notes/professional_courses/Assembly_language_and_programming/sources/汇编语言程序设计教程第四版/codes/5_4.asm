DATA    SEGMENT
X1      DB  63H
X2      DB  31H
X3      DB  91H
X4      DB  78H
Y       DB  ?
DATA    ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,DS:DATA
START:  MOV  AX,DATA
        MOV  DS,AX
        MOV  AL,X1 
        ADD  AL,X2 
        MOV  BL,X3 
        ADD  BL,X4  
        SUB  AL,BL 
        MOV  Y,AL  
        MOV  AH,4CH
        INT  21H
CODE    ENDS
        END  START
