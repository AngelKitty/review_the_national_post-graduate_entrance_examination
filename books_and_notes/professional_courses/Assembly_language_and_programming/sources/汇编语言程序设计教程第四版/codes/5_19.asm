DATA      SEGMENT
DAT       DW  18,8,28,48,38
DATA      ENDS
STACK     SEGMENT  STACK
          DW  200 DUP (?)
STACK     ENDS
CODE      SEGMENT
          ASSUME  CS:CODE,DS:DATA,SS:STACK
START:    MOV  AX,DATA
          MOV  DS,AX
          MOV  CX,5
          DEC  CX                
A1:       MOV  DI,CX
          MOV  BX,0
A2:       MOV  AX,DAT[BX]
          CMP  AX,DAT[BX+2]
          JGE  A3
          XCHG  AX,DAT[BX+2]  
          MOV  DAT[BX],AX
A3:       ADD  BX,2
          LOOP  A2        
          MOV  CX,DI
          LOOP  A1       
          MOV  AH,4CH
          INT  21H
CODE      ENDS
          END  START
