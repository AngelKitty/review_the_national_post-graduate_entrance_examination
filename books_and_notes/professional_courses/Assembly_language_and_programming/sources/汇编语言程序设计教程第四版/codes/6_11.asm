STACK  SEGMENT PARA  STACK 'STACK'
       DW  64 DUP (?)
STACK  ENDS
DATA   SEGMENT
N      DB  8      
FUNCN  DW ?      
DATA   ENDS
CODE   SEGMENT
       ASSUME  CS:CODE,SS:STACK,DS:DATA
MAIN   PROC  FAR
START: PUSH  DS
       MOV  AX,0
       PUSH  AX
       MOV  AX,DATA
       MOV  DS,AX
       PUSH  CX
       MOV  AH,0
       MOV  AL,N      
       CALL  FACTOR     
       MOV  FUNCN,AX   
       POP  CX
       RET
MAIN   ENDP
FACTOR PROC  NEAR
       PUSH  AX
       SUB  AX,1
       JNE  AGAIN        
       POP  AX
       JMP  FIN
AGAIN: CALL  FACTOR   
       POP  CX
       MUL  CL      
FIN:   RET
FACTOR ENDP
CODE   ENDS
       END  START
