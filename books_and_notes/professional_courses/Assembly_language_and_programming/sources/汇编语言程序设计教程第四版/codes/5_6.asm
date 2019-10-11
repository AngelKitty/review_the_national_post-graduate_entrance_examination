DATA      SEGMENT
DAT1      DB  65H
DAT2      DB  ?
DAT3      DB  ?
DATA      ENDS
CODE      SEGMENT
          ASSUME  CS:CODE,DS:DATA
START:    MOV  AX,DATA
          MOV  DS,AX
          LEA  SI,DAT1
          MOV  AL,[SI]
          CMP  AL,99     
          JBE  BCD
          LEA  DI,DAT3
          MOV byte ptr [DI],0FFH
          JMP  EXIT
BCD:      CBW               
          MOV  CL,10
          DIV  CL    
          MOV  CL,4
          SHL  AL,CL  
          OR  AL,AH  
          LEA  DI,DAT2
          MOV  [DI],AL 
EXIT:     MOV  AH,4CH
          INT  21H
CODE      ENDS
          END  START
