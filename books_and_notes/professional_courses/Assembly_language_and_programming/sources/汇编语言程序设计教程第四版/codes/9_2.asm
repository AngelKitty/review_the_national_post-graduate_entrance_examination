DATA    SEGMENT
DIR     DB  30,?,30 DUP(?),0
VIC     DB  'VICTORY',0DH,0AH,'$'     
FAI     DB  'FAILURE',0DH,0AH,'$'   
DATA    ENDS
STACK   SEGMENT  STACK
        DW  100 DUP(?)
STACK   ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,SS:STACK,DS:DATA
START:  MOV  AX,DATA
        MOV  DS,AX
        MOV  AH,0AH             
        LEA  DX,DIR
        INT  21H
        MOV  BL,DIR+1
        CMP  BL,0
        JZ  STOP               
        XOR  BH,BH
        MOV  DIR[BX+2],0    
        MOV  DX,OFFSET DIR+2
        MOV  AH,39H             
        INT  21H
        JC  DD              
        LEA  DX,VIC
        JMP  DDD
DD:     LEA  DX,FAI
DDD:    MOV  AH,9       
        INT  21H
STOP:   MOV  AH,4CH
        INT  21H
CODE    ENDS
        END  START
