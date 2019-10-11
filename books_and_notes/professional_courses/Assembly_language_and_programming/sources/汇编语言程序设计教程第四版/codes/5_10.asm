STACK   SEGMENT PARA STACK 'STACK'
        DB  256 DUP (?)
STACK   ENDS
DATA    SEGMENT
STR1    DB  'Branch1','$'
STR2    DB  'Branch2','$'
STR3    DB  'Branch3','$'
STR4    DB  'Branch4','$'
ERR     DB  'Error','$'
DATA    ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,SS:STACK,DS:DATA
START:  MOV  AX,DATA
        MOV  DS,AX
BEGIN:  MOV  AH,01H  
        INT  21H
        CMP  AL,41H   
        JE  A1
        CMP  AL,42H  
        JE  A2
        CMP  AL,43H 
        JE  A3
        CMP  AL,44H 
        JE  A4
        MOV  DX,OFFSET ERR  
        MOV  AH,9
        INT  21H
        JMP  FINISH   
A1:     MOV  DX,OFFSET STR1  
        MOV  AH,9
        INT  21H
        JMP  ENTER
A2:     MOV  DX,OFFSET STR2   
        MOV  AH,9
        INT  21H
        JMP  ENTER
A3:     MOV  DX,OFFSET STR3 
        MOV  AH,9
        INT  21H
        JMP  ENTER
A4:     MOV  DX,OFFSET STR4  
        MOV  AH,9
        INT  21H
ENTER:  MOV  DL,0DH  
        MOV  AH,2
        INT  21H
        MOV  DL,0AH
        INT  21H
        JMP  BEGIN   
FINISH: MOV  AH,4CH
        INT  21H
CODE    ENDS
        END  START
