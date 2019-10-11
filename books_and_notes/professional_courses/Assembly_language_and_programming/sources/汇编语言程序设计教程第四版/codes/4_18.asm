DATA       SEGMENT
DATABUF    DB  06H,46H,07CH
SUM        DB  ?
DATA       ENDS
CODE       SEGMENT
           ASSUME  CS:CODE,DS:DATA
ADDP       PROC FAR
START:     PUSH  DS
           MOV  AX,0
           PUSH  AX
           MOV  AX,DATA
           MOV  DS,AX
           MOV  AL,0
           MOV  SI,OFFSET DATABUF
           ADD  AL,[SI]
           INC  SI
           ADD  AL,[SI]
           INC SI
           ADD  AL,[SI]
           MOV  SUM,AL
           RET
ADDP       ENDP
CODE       ENDS
           END  START
