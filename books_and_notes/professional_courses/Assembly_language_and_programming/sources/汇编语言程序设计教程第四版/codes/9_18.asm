FRAME      STRUC            
SAVEBP     DW  ?            
SAVECSIP   DW  2 DUP(?)   
N          DW  ?           
M          DW  ?   
READDR     DW  ?      
FRAME      ENDS
DATA       SEGMENT
MM         DW  10
NN         DW  100
RESULT     DW  ?
DATA       ENDS
STACK      SEGMENT  STACK
           DW  100 DUP(?)
STACK      ENDS
CODE       SEGMENT
           ASSUME  CS:CODE,SS:STACK,DS:DATA
START:     MOV  AX,DATA
           MOV  DS,AX
           LEA  SI,RESULT   
           PUSH  SI
           MOV  AX,MM
           PUSH  AX
           MOV  BX,NN
           PUSH  BX
           CALL  FAR PTR ACK   
           MOV  AH,4CH
           INT  21H
ACK        PROC  FAR
           PUSH  BP 
           MOV  BP,SP   
           MOV  SI,[BP].READDR     
           MOV  AX,[BP].M        
           MOV  BX,[BP].N       
           CMP  AX,0       
           JA  TESTN
           INC  BX                  
           MOV  [SI],BX     
           JMP  EXIT2
TESTN:     CMP  BX,0         
           JA  A1

           DEC  AX                 
           MOV  BX,1

           PUSH  SI
           PUSH  AX
           PUSH  BX
           CALL  ACK              
           JMP  EXIT2

A1:        PUSH  SI
           PUSH  AX
           DEC  BX
           PUSH  BX
           CALL  ACK         

           PUSH  SI                     
           MOV  AX,[BP].M
           DEC  AX            
           PUSH  AX
           MOV  DX,[SI]        
           PUSH  DX
           CALL  ACK         
EXIT2:     POP  BP
           RET  6
ACK        ENDP
CODE       ENDS 
           END  START
