STACK        SEGMENT  STACK
             DW  64 DUP(?)
STACK        ENDS
DATA         SEGMENT
STR1         DB  'STUDENT'
STR2         DB  'T',01H,'E',0FH,'A',01H
             DB  'C',0FH,'H',01H,'E',0FH,'R',01H
LEN2         EQU  $-STR2
DATA         ENDS
CODE         SEGMENT
             ASSUME  CS:CODE,SS:STACK,DS:DATA
START:       MOV  AX,DATA
             MOV  DS,AX
             MOV  AL,3    
             MOV  AH,0
             INT  10H
             MOV  BP,SEG STR1    
             MOV  ES,BP
             MOV  BP,OFFSET STR1  
             MOV  CX,STR2-STR1  
             MOV  DX,0
             MOV  BL,0FH
             MOV  AL,1
             MOV  AH,13H
             INT  10H
             MOV  AH,3
             INT  10H
             MOV  BP,OFFSET STR2  
             MOV  CX,LEN2
             MOV  AL,3
             MOV  AH,13H
             INT  10H
             MOV  AH,4CH    
             INT 21H
CODE         ENDS
             END  START
