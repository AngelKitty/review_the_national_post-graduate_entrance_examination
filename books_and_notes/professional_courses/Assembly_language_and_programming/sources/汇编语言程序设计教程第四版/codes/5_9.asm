STACK    SEGMENT  STACK
         DW  100 DUP (?)
STACK    ENDS
CODE     SEGMENT
         ASSUME  CS:CODE,SS:STACK
START:   MOV  AH,1   
         INT  21H
         CLC
         RCR  AL,1   
         JNC  EVN
         MOV  BL,31H 
         JMP  DISPLAY
EVN:     MOV  BL,30H  
DISPLAY: MOV  AH,2
         MOV  DL,0DH   
         INT  21H
         MOV  DL,0AH   
         INT  21H
         MOV  DL,BL
         INT  21H
         MOV  AH,4CH
         INT  21H
CODE     ENDS
         END  START
