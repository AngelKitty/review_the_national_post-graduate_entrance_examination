INCLUDE    DISP.MAC   
INCLUDE    SUB.ASM     
STACK      SEGMENT  STACK
           DW  256 DUP(?)
STACK      ENDS
DATA       SEGMENT
MSG1       DB  'ENTER NUMBER(XX):'0DH,0AH,'$'
MSG2       DB  'THE NUMBERS ENTERED ARE:'0DH,0AH,'$'
MSG3       DB  'THE SORTING RESULT:'0DH,0AH,'$'
CRLF       DB  0DH,0AH,'$'
MAXCOUNT   =80                
COUNT      DB  ?             
BUF        DB  MAXCOUNT DUP(?)  
DATA       ENDS
CODE       SEGMENT
           ASSUME  CS:CODE,SS:STACK,DS:DATA
START:     MOV  AX,DATA
           MOV  DS,AX
           DISPMSG  MSG1           
           MOV  BX,OFFSET BUF
           CALL  INPUT           
           CMP  CX,0            
           JE  S4
           MOV  COUNT,CX      
           DISPMSG  MSG2          
           MOV  BX,OFFSET BUF
           MOV  CX,COUNT
S2:        MOV  AL,[BX]
           CALL  ALDISP
           DISPCHAR ','
           INC  BX
           LOOP  S2
           DISPMSG  CRLF
           MOV  BX,OFFSET BUF
           MOV  CX,COUNT
           CALL  SORTING          
           DISPMSG  MSG3           
           MOV  BX,OFFSET BUF
           MOV  CX,COUNT
S3:        MOV  AL,[BX]
           CALL  ALDISP
           DISPCHAR  ','
           INC  BX
           LOOP  S3
           DISPMSG  CRLF
S4:        MOV  AH,4CH
           INT  21H
CODE       ENDS
           END  START
