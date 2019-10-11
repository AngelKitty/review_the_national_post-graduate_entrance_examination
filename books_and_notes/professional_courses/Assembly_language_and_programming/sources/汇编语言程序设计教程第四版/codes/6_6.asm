DATA   SEGMENT
BUF    DB  10000101B    
DATA   ENDS
STACK  SEGMENT  STACK
       DW  200 DUP (?)
TOP    LABEL  WORD
STACK  ENDS
CODE   SEGMENT
       ASSUME  CS:CODE,SS:STACK,DS:DATA
START: MOV  AX,DATA
       MOV  DS,AX
       MOV  AX,STACK
       MOV  SS,AX
       MOV  SP,OFFSET TOP
       MOV  AH,00H
       MOV  AL,BUF
       PUSH  AX           
       CALL  SUBP   
       POP  AX       
       MOV  BX,AX
       MOV  CH,2
LP1:   MOV  CL,4
       ROL  BL,CL        
       MOV  AL,BL
       AND  AL,0FH  
       ADD  AL,30H   
       CMP  AL,3AH
       JL   LP2
       ADD  AL,07H
LP2:   MOV  DL,AL
       MOV  AH,2      
       INT  21H
       DEC  CH
       JNZ  LP1
       MOV  AH,4CH
       INT  21H
SUBP   PROC  NEAR
       PUSH  AX
       PUSHF
       PUSH  BX
       PUSH  CX
       PUSH  BP
       MOV  BP,SP
       MOV  AX,[BP+12] 
       MOV  AH,AL
       AND  AH,0FH  
       MOV  BL,AH 
       AND  AL,0F0H       
       MOV  CL,4
       ROR  AL,CL
       MOV  BH,0AH    
       MUL  BH
       ADD  AL,BL
       MOV  [BP+12],AX   
       POP  BP
       POP  CX
       POP  BX
       POPF
       POP  AX
       RET
SUBP   ENDP
CODE   ENDS
       END  START
