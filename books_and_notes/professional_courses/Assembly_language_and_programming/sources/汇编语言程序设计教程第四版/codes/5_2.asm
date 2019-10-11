DATA      SEGMENT
BUF       DB  68H  
BUF1      DB  ?  
BUF2      DB  ?  
BUF3      DB  ?  
BUF4      DB  ?  
DATA      ENDS
CODE      SEGMENT
          ASSUME  CS:CODE,DS:DATA
MAIN      PROC  FAR
START:    PUSH  DS
          MOV  AX,0
          PUSH  AX
          MOV  AX,DATA
          MOV  DS,AX
          MOV  AL,BUF
          MOV  CL,4
          SHR  AL,CL   
          MOV  BUF2,AL
          OR  AL,30H   
          MOV  BUF4,AL
          MOV  AL,BUF
          AND  AL,0FH   
          MOV  BUF1,AL
          OR  AL,30H    
          MOV  BUF3,AL
          RET
MAIN      ENDP
CODE      ENDS
          END  START
