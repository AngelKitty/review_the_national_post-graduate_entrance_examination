DATA       SEGMENT
BUFF       DB  60       
NUM        DB  ?
CHARS      DB  60 DUP(?)
MESG       DB  0DH,0AH
           DB  'What is your name?:$'
DATA       ENDS
CODE       SEGMENT
           ASSUME  CS:CODE,DS:DATA
START:     MOV  AX,DATA
           MOV  DS,AX
           MOV  DX,OFFSET MESG  
           MOV  AH,9
           INT  21H
           MOV  DX,OFFSET BUFF    
           MOV  AH,0AH
           INT  21H
           XOR  BX,BX
           MOV  BL,NUM            
           MOV  CHARS[BX],'$'   
           MOV  DX,OFFSET CHARS  
           MOV  AH,9
           INT  21H
           MOV  AH,4CH     
           INT  21H
CODE       ENDS
           END  START
