DATA    SEGMENT
NUM     DW  ?
SUM     DW  ?
DATA    ENDS
STACK   SEGMENT  STACK
        DW  100 DUP (?)
STACK   ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,SS:STACK,DS:DATA
START:  MOV  AX,DATA
        MOV  DS,AX
        MOV  AX,0  
        MOV  BX,0
AGAIN:  INC  BX
        ADD  AX,BX
        CMP  AX,600  
        JBE  AGAIN   
        MOV  SUM,AX  
        MOV  NUM,BX 
        MOV  AH,4CH
        INT  21H
CODE    ENDS
        END  START
