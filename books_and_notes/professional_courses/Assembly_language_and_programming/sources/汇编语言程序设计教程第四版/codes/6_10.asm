CODE       SEGMENT
           ASSUME  CS:CODE
START:     MOV  AH,1 
           INT  21H
           CALL  SUB1
           MOV  AH,4CH
           INT  21H
SUB1       PROC  NEAR    
           CMP  AL,30H
           JB  NEXT
           CMP  AL,46H
           JA  NEXT
           CMP  AL,40H
           JA  AA         
           CMP  AL,39H
           JBE  AA        
           JMP  NEXT
AA:        CALL  SUB2
           JMP  DONE
NEXT:      MOV  AL,0FFH
DONE:      RET
SUB1       ENDP
SUB2       PROC  NEAR    
           CMP  AL,39H
           JBE  BB
           SUB  AL,07H
BB:        SUB  AL,30H
           RET
SUB2       ENDP
CODE       ENDS
           END  START
