DATA       SEGMENT
STRING     DB  0DH,0AH,'THIS IS A MUSIC PROGRAM!$'
MDATA1     DW  330,392,330,294,330,392,330,294,330  
           DW  330,292,330,294,262,294,330,392,294
           DW  262,262,220,196,196,220,262,294,332,262,-1
MDATA2     DW  50,50,50,25,25,50,25,25,100    
           DW  50,50,25,25,50,50,25,25,100
           DW  50,25,25,50,25,25,50,25,25,100
DATA       ENDS
STACK      SEGMENT  STACK
           DW  100  DUP (?)
STACK      ENDS
CODE       SEGMENT
           ASSUME  DS:DATA,SS:STACK,CS:CODE
MAIN       PROC  FAR
           MOV  AX,DATA
           MOV  DS,AX
           LEA  DX,STRING   
           MOV  AH,9
           INT  21H
           CALL  MPLAY    
           MOV  AH,4CH
           INT  21H
MAIN       ENDP
MPLAY      PROC  NEAR
           LEA  SI,MDATA1     
           LEA  BP,DS:MDATA2    
LOP:       MOV  DI,[SI]
           CMP  DI,-1            
           JE  ENDPLAY
           MOV  BX,DS:[BP]  
           CALL  SOUNDPLAY    
           ADD  SI,2      
           ADD  BP,2      
           JMP  LOP
ENDPLAY:   RET
MPLAY      ENDP
SOUNDPLAY  PROC  NEAR
           PUSH  AX
           PUSH  BX
           PUSH  CX
           PUSH  DX
           PUSH  DI
           MOV  AL,0B6H    
           OUT  43H,AL   
           MOV  DX,12H
           MOV  AX,533H*896
           DIV  DI            
           OUT  42H,AL
           MOV  AL,AH
           OUT  42H,AL
           IN  AL,61H      
           MOV  AH,AL    
           OR  AL,3
           OUT  61H,AL  
WAIT1:     MOV  CX,8FF0H
DELAY1:    LOOP  DELAY1
           DEC  BX
           JNZ  WAIT1
           MOV  AL,AH    
           OUT  61H,AL
           POP  DI
           POP  DX
           POP  CX
           POP  BX
           POP  AX
           RET
SOUNDPLAY  ENDP
CODE       ENDS
           END  MAIN
