DATA   SEGMENT
X1     DW  32,45,34,54,23,44,22,44
X2     DW  70,61,26,35,28,67,91,10
Y      DW  8 DUP (?)
DATA   ENDS
STACK  SEGMENT  STACK
       DW  200 DUP (?)
STACK  ENDS
CODE   SEGMENT
       ASSUME  CS:CODE,SS:STACK,DS:DATA
START: MOV  AX,DATA
       MOV  DS,AX
       LEA  BX,X1        
       LEA  SI,X2 
       LEA  DI,Y     
       MOV  CX,8       
AGAIN: MOV  AX,[BX]  
       MOV  DX,[SI]  
       CALL  SUBPRO    
       MOV  [DI],AX    
       ADD  BX,2     
       ADD  SI,2
       ADD  DI,2
       LOOP  AGAIN
       MOV  AH,4CH
       INT  21H
SUBPRO PROC NEAR
       PUSH  BX
       PUSH  CX
       MOV  BX,DX
       ADD  AX,DX      
       SAL  AX,1    
       ADD  BX,9    
       SUB  AX,BX    
       POP  CX
       POP  BX
       RET
SUBPRO ENDP
CODE   ENDS
       END  START
