STACK    SEGMENT STACK
DW       100 DUP (0)
STACK    ENDS
DATA     SEGMENT
COUNT    DW  91      
DATA     ENDS
CODE     SEGMENT
         ASSUME  CS:CODE,DS:DATA,SS:STACK
MAIN     PROC  FAR    
         MOV  AX,DATA
         MOV  DS,AX
         MOV  AH,35H     
         MOV  AL,1CH
         INT  21H
         PUSH  ES          
         PUSH  BX
         PUSH  DS
         MOV  DX,SEG INT1CH
         MOV  DS,DX
         LEA  DX,INT1CH
         MOV  AH,25H        
         MOV  AL,1CH
         INT  21H
         POP  DS
         IN  AL,21H
         AND  AL,0FCH       
         OUT  21H,AL
         STI                 
         MOV  AH,6        
         MOV  AL,0
         MOV  BH,1FH
         MOV  CX,0
         MOV  DX,184FH
         INT  10H
         MOV  AL,40H        
PRINT0:  PUSH  AX       
         MOV  AH,1        
         INT  21H
         OR  AL,20H       
         CMP  AL,'q' 
         POP  AX              
         JE  EXIT0        
         INC  AL         
         CMP  AL,'Z'
         JNA  GONEXT
         MOV  AL,41H
GONEXT:  MOV  DX,0002H        
         MOV  BH,0            
PRINT10: INC  DH      
         CMP  DH,24
         JA  PRINT0            
         ADD  DL,3            
         MOV  AH,2              
         INT  10H
         MOV  AH,9                
         MOV  BL,1FH            
         MOV  CX,1
         INT  10H
         JMP  PRINT10          
EXIT0:   POP  DX           
         POP  DS
         MOV  AH,25H
         MOV  AL,1CH
         INT  21H
         MOV  AH,4CH 
         INT  21H
MAIN     ENDP
INT1CH   PROC  FAR    
         PUSH  AX       
         PUSH  BX
         PUSH  CX
         PUSH  DX
         PUSH  SI
         PUSH  DI
         PUSH  BP
         PUSH  DS
         PUSH  ES
         STI             
         MOV  AX,DATA
         MOV  DS,AX
         DEC  COUNT
         JNZ  EXIT
         MOV  AH,2
         MOV  BH,0
         MOV  DL,79
         INT  10H
         MOV  AH,0EH
         MOV  BH,0
         MOV  AL,'V'   
         INT  10H
         MOV  COUNT,91     
EXIT:    CLI             
         POP  ES                
         POP  DS
         POP  BP
         POP  DI
         POP  SI
         POP  DX
         POP  CX
         POP  BX
         POP  AX
         IRET                 
INT1CH   ENDP
CODE     ENDS
         END  MAIN
