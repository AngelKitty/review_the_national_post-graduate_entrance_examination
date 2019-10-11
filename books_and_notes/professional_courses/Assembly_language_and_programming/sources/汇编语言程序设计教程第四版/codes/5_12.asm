STACK    SEGMENT  STACK
         DW  64 DUP (?)
STACK    ENDS
DATA     SEGMENT
BUF      DB  7,44,-12,67,-10,-72,84,-30,?
DATA     ENDS
CODE     SEGMENT
         ASSUME  CS:CODE,DS:DATA,SS:STACK
START:   MOV  AX,DATA
         MOV  DS,AX
         MOV  DI,OFFSET  BUF
         MOV  CL,[DI]     
         XOR  CH,CH
         MOV  BL,CH
         INC  DI
A1:      MOV  AL,[DI]
         TEST  AL,80H 
         JZ  A2
         INC  BL   
A2:      INC  DI
         LOOP  A1
         MOV  [DI],BL  
         MOV  AH,4CH
         INT  21H
CODE     ENDS
         END  START
