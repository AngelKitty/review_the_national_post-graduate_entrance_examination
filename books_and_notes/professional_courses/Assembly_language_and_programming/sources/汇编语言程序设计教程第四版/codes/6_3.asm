CODE        SEGMENT
            ASSUME  CS:CODE
START:      MOV  BL,2   
            DEC  BL
            PUSH  BX
PUT1:       MOV  DL,20H    
            MOV  AH,2
            INT  21H
            POP  BX
            MOV  AL,BL
            INC  AL       
            DAA
            AND  AL,0FH     
            CMP  AL,00H
            JNZ  NEXT
            MOV  AL,2
NEXT:       MOV  BL,AL
            PUSH  BX
            OR  AL,30H     
            MOV  DL,AL   
            MOV  AH,2
            INT  21H
            CALL  DELAY   
            JMP  PUT1
            MOV  AH,4CH
            INT  21H
DELAY       PROC  NEAR    
            PUSH  CX
            MOV  CX,0CFFFH
LOP:        DEC  CX
            JNE  LOP
            POP  CX
            RET
DELAY       ENDP
CODE        ENDS
            END  START
