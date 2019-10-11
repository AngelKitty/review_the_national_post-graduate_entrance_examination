STACK  SEGMENT  STACK
       DB  256 DUP (0)
STACK  ENDS
DATA   SEGMENT
ARY1   DB 10 DUP (3) 
SUM1   DW  ?           
ARY2   DB  8 DUP (5)  
SUM2   DW  ?          
ARY3   DB  6 DUP (8)  
SUM3   DW  ?       
DATA   ENDS
CODE   SEGMENT
       ASSUME  CS:CODE,DS:DATA,SS:STACK
START: MOV   AX,DATA
       MOV   DS,AX
       MOV   AX,SIZE ARY1    
       PUSH   AX          
       MOV   AX,OFFSET ARY1   
       PUSH   AX      
       CALL   SUM       
       MOV   AX,SIZE ARY2 
       PUSH   AX        
       MOV   AX,OFFSET ARY2     
       PUSH   AX    
       CALL   SUM     
       MOV   AX,SIZE ARY3  
       PUSH   AX         
       MOV   AX,OFFSET ARY3   
       PUSH   AX       
       CALL   SUM      
       MOV  AH,4CH
       INT  21H
SUM    PROC  NEAR
       PUSH    AX
       PUSH    BX
       PUSH    CX
       PUSH    BP
       MOV    BP,SP
       PUSHF
       MOV    CX,[BP+12]  
       MOV    BX,[BP+10]  
       MOV    AX,0
AGAIN: ADD  AL,[BX]   
       INC     BX          
       ADC    AH,0
       LOOP   AGAIN
       MOV    [BX],AX    
       POPF
       POP    BP
       POP    CX
       POP    BX
       POP    AX
       RET    4
SUM    ENDP
CODE   ENDS
       END   START
