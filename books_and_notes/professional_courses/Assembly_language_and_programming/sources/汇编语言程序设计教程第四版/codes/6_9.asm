DATA      SEGMENT
STR0      DB  "INPUT THE RIGHT CHARACTER",'$'
STR1      DB  'INPUT CHARACTER:$'
STR2      DB  'THE FIRST SUBROUTINE!$'
STR3      DB  'THE SECOND SUBROUTINE!$'
DATA      ENDS
CODE      SEGMENT
          ASSUME  CS:CODE,DS:DATA
MAIN      PROC  FAR
START:    PUSH  DS
          MOV  AX,0
          PUSH  AX
          MOV  AX,DATA
          MOV  DS,AX
          MOV  DX,OFFSET STR1
          MOV  AH,09H
          INT  21H      
          MOV  AH,01H
          INT  21H
          CMP  AL,'1'
          JE  SUBF          
          CMP  AL,'2'
          JE  SUBS        
          CALL  ENTER
          LEA  DX,STR0
          MOV  AH,09H
          INT  21H     
          JMP  RETN
SUBF:     CALL  FIRST
          JMP  RETN
SUBS:     CALL  SECOND
RETN:     RET
MAIN      ENDP
ENTER     PROC
          MOV  DL,0DH
          MOV  AH,02H
          INT  21H
          MOV  DL,0AH
          MOV  AH,02H
          INT  21H      
          RET
ENTER     ENDP
FIRST     PROC
          CALL  ENTER
          LEA  DX,STR2
          MOV  AH,09H
          INT  21H
          RET
FIRST     ENDP
SECOND    PROC
          CALL  ENTER
          LEA  DX,STR3
          MOV  AH,09H
          INT  21H
          RET
SECOND    ENDP
CODE      ENDS
          END  START
