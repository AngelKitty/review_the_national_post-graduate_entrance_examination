DATA   SEGMENT
COMP   STRUC       
RE     DW  0
IM     DW  0
COMP   ENDS
CNUM1  COMP <10,30>       
CNUM2  COMP <20,40>
CNUM3  COMP <0,0>
DATA   ENDS
CODE   SEGMENT
       ASSUME  CS:CODE,DS:DATA
START: PUSH  DS
       MOV  AX,0
       PUSH  AX
       MOV  AX,DATA
       MOV  DS,AX
       MOV  BX,OFFSET CNUM3     
       MOV  AX,CNUM1.RE
       ADD  AX,CNUM2.RE     
       MOV  [BX].RE,AX
       MOV  AX,CNUM1.IM
       ADD  AX,CNUM2.IM     
       MOV  [BX].IM,AX
       RET
CODE   ENDS
       END  START
