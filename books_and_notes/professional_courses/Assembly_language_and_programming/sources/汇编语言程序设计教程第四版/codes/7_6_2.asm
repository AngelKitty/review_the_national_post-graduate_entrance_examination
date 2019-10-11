;INT4AH的处理程序
CSEG       SEGMENT
           ASSUME  CS:CSEG
           PUBLIC  INT4AH
INT4AH     PROC  FAR
           PUSH  DX      
           PUSH  CX
           PUSH  SI
           MOV  CX,0         
           MOV  SI,AX   
LP:        MOV  AL,[SI]
           CMP  AL,'$' 
           JZ  OVER
           MOV  DL,AL   
           MOV  AH,2
           INT  21H
           INC  SI
           INC  CX       
           JMP  LP
OVER:      MOV  AX,CX
           CMP  AH,0
           JE  DONE         
           MOV  AH,0FFH    
DONE:      POP  SI          
           POP  CX
           POP  DX
           IRET          
INT4AH     ENDP
CSEG       ENDS
           END