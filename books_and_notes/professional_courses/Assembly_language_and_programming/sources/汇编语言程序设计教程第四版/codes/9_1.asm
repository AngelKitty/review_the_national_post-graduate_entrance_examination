DATA       SEGMENT
BUFFER     DB  80,?,80 DUP(?)
DATA       ENDS
CODE       SEGMENT
           ASSUME  CS:CODE,DS:DATA
START:     MOV  AX,DATA
           MOV  DS,AX
           MOV  DX,OFFSET  BUFFER     
           MOV  AH,0AH            
           INT  21H
           MOV  AH,4CH
           INT  21H
CODE       ENDS
           END  START
