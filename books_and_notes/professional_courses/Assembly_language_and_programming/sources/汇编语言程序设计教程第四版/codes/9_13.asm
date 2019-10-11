DATA     SEGMENT
MODE     DB ?            
DATA     ENDS
CODE     SEGMENT
         ASSUME  CS:CODE,DS:DATA
START:   MOV  AX,DATA
         MOV  DS,AX
         MOV  AH,0FH
         INT  10H
         MOV  MODE,AL     
         MOV  AH,0
         MOV  AL,13H
         INT  10H
         MOV  CX,0         
         MOV  AL,01H              
DLINE:   CALL  VLINE
         INC  AL                    
         INC  CX                   
         CMP  CX,320                
         JL  DLINE
         MOV  AH,0
         INT  16H                   
         MOV  AL,MODE
         MOV  AH,0
         INT  10H                   
         MOV  AH,4CH
         INT  21H
VLINE    PROC  NEAR       
         MOV  DX,0
         MOV  BH,0
         MOV  AH,0CH
DRAW:    INT  10H
         INC  DX
         CMP  DX,200            
         JL  DRAW
         RET
VLINE    ENDP
CODE     ENDS
         END  START
