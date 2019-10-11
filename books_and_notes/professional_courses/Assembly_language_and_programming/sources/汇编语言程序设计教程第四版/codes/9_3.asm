STACK   SEGMENT  STACK
        DW  100 DUP(?)
STACK   ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,SS:STACK
START:  MOV  AH,1  
        INT  21H
        SUB  AL,20H     
        CMP  AL,'A'
        JB  STOP         
        JZ  ZAB           
        CMP  AL,'Z'
        JA  STOP          
        JZ  YZA           
        DEC  AL         
        MOV  DL,AL
        MOV  CX,3
AGAIN:  CALL  DISCH
        INC  DL
        LOOP  AGAIN
        JMP  STOP
ZAB:    MOV  BL,AL        
        ADD  AL,25          
        MOV  DL,AL
        CALL  DISCH
        MOV  DL,BL
        CALL  DISCH
        INC  DL
        CALL  DISCH
        JMP  STOP
YZA:    MOV  BL,AL   
        DEC  AL
        MOV  DL,AL
        CALL  DISCH
        INC  DL
        CALL  DISCH
        SUB  DL,25          
        CALL  DISCH
STOP:   MOV  AH,4CH
        INT  21H
DISCH   PROC  NEAR     
        MOV  AH,2
        INT  21H
        RET
DISCH   ENDP
CODE    ENDS
        END  START
