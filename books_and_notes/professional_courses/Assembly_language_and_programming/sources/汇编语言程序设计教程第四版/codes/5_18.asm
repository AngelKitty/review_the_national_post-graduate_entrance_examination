DATA    SEGMENT
GRADE   DB  85,73,92,81,96 
        DB  100,86,95,77,80
        DB  56,71,69,80,68
        DB  83,78,69,90,84
AVEBUF  DB  5 DUP(?)
DATA    ENDS
CODE   SEGMENT
       ASSUME  CS:CODE,DS:DATA
START: MOV  AX,DATA
       MOV  DS,AX
       MOV  BX,OFFSET GRADE  
       MOV  DI,OFFSET AVEBUF  
       MOV  SI,BX
       MOV  CX,5     
A1:    MOV  BX,SI
       SUB  BX,5
       PUSH  CX     
       MOV  AL,0
       MOV  CX,4     
A2:    ADD  BX,5      
       ADD  AL,[BX]
       LOOP  A2
       MOV  AH,0
       MOV  DL,4   
       DIV  DL     
       MOV  [DI],AL 
       INC  SI
       INC  DI
       POP  CX    
       LOOP  A1
       MOV  AH,4CH
       INT  21H
CODE   ENDS
       END  START