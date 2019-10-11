DATA    SEGMENT
DAT     DB  0,1,4,9,16,25,36,49
        DB  64,81,100,121,144,169,196,225
SQU     DB  10
DATA    ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,DS:DATA
START:  MOV  AX,DATA
        MOV  DS,AX
        MOV  BX,OFFSET DAT
        XOR  AX,AX
        MOV  AL,SQU 
        ADD  BX,AX   
        MOV  AL,[BX]  
        MOV  SQU,AL   
        MOV  AH,4CH
        INT  21H
CODE    ENDS
        END  START
