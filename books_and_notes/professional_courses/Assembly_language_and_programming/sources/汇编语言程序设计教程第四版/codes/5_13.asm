STACK   SEGMENT  STACK
        DB  256 DUP (?)
STACK   ENDS
DATA    SEGMENT
ASCBUF  DB  30H,32H,34H,54H,37H,20H,33H 
        DB  7 DUP (?)
DATA    ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,DS:DATA,SS:STACK
START:  MOV  AX,DATA
        MOV  DS,AX
        MOV  CX,7
        MOV  DI,OFFSET ASCBUF  
LAB1:   MOV  BL,0FFH   
        MOV  AL,[DI]
        CMP  AL,3AH
        JNB  OK
        SUB  AL,30H   
        JC  OK
        MOV  BL,AL
OK:     MOV  AL,BL
        MOV  [DI+06H],AL  
        INC  DI
        LOOP  LAB1
        MOV  AH,4CH
        INT  21H
CODE    ENDS
        END  START
