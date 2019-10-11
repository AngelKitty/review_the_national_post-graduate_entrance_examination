CODE     SEGMENT
         ASSUME  CS:CODE,DS:CODE
ORG      100H
START:   JMP  BEGIN
A        DW  3CC8H
         DW  ?
BEGIN:   MOV  AX,CS
         MOV  DS,AX
         LEA  BX,A
         MOV  AX,[BX]
         AND  AX,0FF0H
         INC  BX
         INC  BX
         MOV  [BX],AX
         MOV  AH,4CH
         INT  21H
CODE     ENDS
         END  START
