DATA   SEGMENT
COUNT  EQU  10     
ARRAY  DB  36H,8AH,26H,37H,99H,0DH,62H,0FH,50H,43H
RESULT DB  ?    
DATA   ENDS
STACK  SEGMENT  STACK
       DW  100 DUP (?)
STACK  ENDS
CODE   SEGMENT
       ASSUME  CS:CODE,SS:STACK,DS:DATA
START: MOV  AX,DATA
       MOV  DS,AX
       CALL  SUBP         
       MOV  AH,4CH
       INT  21H
SUBP   PROC  NEAR
       PUSH  AX
       PUSH  BX
       PUSH  CX
       XOR  AL,AL         
       MOV  BX,OFFSET ARRAY   
       MOV  CX,COUNT       
SUMB:  ADD  AL,[BX]    
       INC  BX             
       LOOP  SUMB
       MOV  RESULT,AL  
       POP  CX
       POP  BX
       POP  AX
       RET
SUBP   ENDP
CODE   ENDS
       END  START
