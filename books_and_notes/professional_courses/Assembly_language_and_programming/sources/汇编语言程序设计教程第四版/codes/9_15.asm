DATA       SEGMENT
FILE       DB  'A:TEXTFILE.ASC',0
BUFFER     DB  128 DUP(?)
ERRMSG     DB  'ERROR IN READ FILE.','$'
DATA       ENDS
CODE       SEGMENT
           ASSUME  CS:CODE,DS:DATA,ES:DATA
START:     MOV  AX,DATA
           MOV  DS,AX
           MOV  ES,AX
           MOV  AH,3DH      
           LEA  DX,FILE   
           MOV  AL,0     
           INT  21H
           MOV  BX,AX           
READIN:    LEA  DX,BUFFER
           MOV  CX,0010H     
           PUSH  CX
           MOV  AX,3FH
           INT  21H        
           LEA  DX,ERRMSG
           JC  ERR          
           OR  AX,AX
           JE  EXIT        
           POP  CX
           XOR  SI,SI
DISPLAY:   MOV  DL,BUFFER[SI]
           CMP  DL,1AH                 
           JE  EXIT
           MOV  AH,2
           INT  21H
           INC  SI
           LOOP  DISPLAY
           JMP  READIN
ERR:       MOV  AH,9
           INT  21H
EXIT:      MOV  AH,3EH       
           INT  21H               
           MOV  AH,4CH    
           INT  21H
CODE       ENDS
           END  START
