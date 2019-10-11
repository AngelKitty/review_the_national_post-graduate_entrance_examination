STACK      SEGMENT  STACK
           DW  64 DUP(?)
STACK      ENDS
CODE       SEGMENT
           ASSUME  CS:CODE,SS:STACK
START:     PUSH  DS 
           MOV  AX,0
           PUSH  AX
           MOV  AH,0FH       
           INT  10H
           MOV  AH,0       
           MOV  AL,3
           INT  10H
           MOV  CX,1         
           MOV  DX,0         
LOP:       MOV  AH,2          
           INT  10H
           MOV  AL,40H
           MOV  AH,0AH     
           INT  10H
           INC  DH        
           INC  DL
           CMP  DH,19     
           JNE  LOP
           RET
CODE       ENDS
           END    START
