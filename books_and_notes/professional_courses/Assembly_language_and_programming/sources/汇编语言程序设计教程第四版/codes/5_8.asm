DATA   SEGMENT
CHA1   DB  'A'
CHA2   DB  'B'
STR1   DB  'YES','$' 
STR2   DB  'NO','$'
DATA   ENDS
STACK  SEGMENT  STACK
       DW  100 DUP (?)
STACK  ENDS
CODE   SEGMENT
       ASSUME  CS:CODE,SS:STACK,DS:DATA
START: MOV  AX,DATA
       MOV  DS,AX
       MOV  AL,CHA1
       MOV  BL,CHA2
       CMP  AL,BL     
       JNE  NEXT1
       LEA  DX,STR1  
       JMP  NEXT2
NEXT1: LEA  DX,STR2
NEXT2: MOV  AH,9   
       INT  21H
       MOV  AH,4CH
       INT  21H
CODE   ENDS
       END  START
