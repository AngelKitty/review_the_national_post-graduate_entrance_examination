DATA      SEGMENT
BUFF      DB  8 DUP (?)
DATA      ENDS
CODE      SEGMENT
          ASSUME  CS:CODE,DS:DATA
START:    MOV  AX,DATA
          MOV  DS,AX
          MOV  DL,10H     
          MOV  DI,OFFSET BUFF 
          MOV  CX,8
AGAIN:    MOV  AL,DL
          AND  AL,0EFH  
          OUT  4,AL   
          CALL  DELAY     
          MOV  AL,DL
          OUT  4,AL
A1:       IN  AL,2       
          TEST  AL,01H    
          JZ  A1           
          IN  AL,3         
          MOV  [DI],AL
          INC  DI
          INC  DL        
          LOOP  AGAIN
          MOV  AH,4CH
          INT  21H
CODE      ENDS
          END  START
