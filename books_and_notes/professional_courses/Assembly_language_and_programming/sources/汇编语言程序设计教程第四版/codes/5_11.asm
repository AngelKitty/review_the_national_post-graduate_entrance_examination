DATA     SEGMENT
BASE     DW  SUBP1,SUBP2,SUBP3,SUBP4 
         DW  SUBP5,SUBP6,SUBP7,SUBP8
DISPLAY  DB  'ERROR',0AH,0DH,'$'
DATA     ENDS
STACK    SEGMENT  STACK
         DW  100 DUP (?)
STACK    ENDS
CODE     SEGMENT
         ASSUME  CS:CODE,SS:STACK,DS:DATA
START:   MOV  AX,DATA
         MOV  DS,AX
INPUT:   MOV  AH,1    
         INT  21H
         CMP  AL,'1' 
         JB  ERR    
         CMP   AL,'8'
         JA  ERR
         SUB  AL,'1'
         AND  AX,000FH
         JMP  BASE[AX*2]  
SUBP1: ...  
...
SUBP8: ...
...
ERR:     CMP  AL,'E' 
         JZ  EXIT   
         MOV  DX,OFFSET DISPLAY  
         MOV  AH,09H
         INT  21H
         JMP  INPUT
EXIT:    MOV  AH,4CH
         INT  21H
CODE     ENDS
         END  START
