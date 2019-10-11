DATA   SEGMENT
STUD   RECORD  NUM:4,SEX:1,AGE:6,HOME:5
ARRAY  STUD  30 DUP(< >) 
COUNT  DB  0
DATA   ENDS
CODE   SEGMENT
       ASSUME  CS:CODE,DS:DATA
START: PUSH  DS
       MOV  AX,0
       PUSH  AX
       MOV  AX,DATA
       MOV  DS,AX
       MOV  CH,30
       MOV  BX,OFFSET ARRAY
NEXT:  MOV  AX,[BX]
       TEST  AX,MASK SEX
       JZ  NEXT1              
       MOV  CL,WIDTH HOME 
       MOV  DX,MASK AGE    
       AND  AX,DX       
       SHR  AX,CL       
       CMP  AL,20   
       JL  NEXT1
       INC  COUNT       
NEXT1: ADD  BX,2
       DEC  CH              
       JNE  NEXT
       RET
CODE   ENDS
       END  START
