STACK   SEGMENT  STACK
        DW  64 DUP(?)
STACK   ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,SS:STACK
START:  MOV  CX,001AH           
        MOV  BL,61H     
        MOV  AH,02H   
A1:     MOV  DL,BL
        INT  21H
        INC  BL
        PUSH  CX
        MOV  CX,0FFFFH   
A2:     LOOP  A2
        POP  CX
        DEC  CX
        JNZ  A1
        MOV  AH,4CH
        INT  21H
CODE    ENDS
        END  START
