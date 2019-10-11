DATA    SEGMENT
X1      DW  28H
X2      DW  38H
X3      DW  48H
Y       DW  ?
DATA    ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,DS:DATA
START:  MOV  AX,DATA
        MOV  DS,AX
        MOV  AX,X1 
        ADD  AX,X2 
        ADD  AX,X3 
        MOV  Y,AX  
        MOV  AH,4CH
        INT  21H
CODE    ENDS
        END  START
