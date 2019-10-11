STACK      SEGMENT  STACK
           DW  64 DUP(?)
STACK      ENDS
DATA       SEGMENT
SPORT      DB  17     
           DB  2,7,0,0       
           DB  4FH,7,1,-3
           DB  53H,70H,0,1
           DB  50H,70H,0,1
           DB  4FH,70H,0,1
           DB  52H,70H,0,1
           DB  54H,70H,0,1
           DB  4FH,7,0,1
           DB  0DBH,7,1,-3
           DB  0DBH,7,1,-1
           DB  0DBH,7,0,1
           DB  0DBH,7,0,1
           DB  0DBH,7,0,1
           DB  0DBH,7,1,-4
           DB  0DBH,7,0,4
           DB  4FH,7,1,-5
           DB  4FH,7,0,5 
DATA       ENDS
CODE       SEGMENT
           ASSUME  CS:CODE,DS:DATA,SS:STACK
MAIN       PROC  FAR
           PUSH  DS
           SUB  AX,AX
           PUSH  AX
           MOV  AX,DATA
           MOV  DS,AX
           CALL  CLEAR        
           LEA  SI,SPORT     
           MOV  DH,15          
           MOV  DL,40
           CALL  SPORTDIS        
           RET
MAIN       ENDP
CLEAR      PROC  NEAR        
           PUSH  AX
           PUSH  BX
           PUSH  CX
           PUSH  DX
           MOV  AH,6    
           MOV  AL,0
           MOV  CX,0     
           MOV  DH,24   
           MOV  DL,79
           MOV  BH,7
           INT  10H
           POP  DX
           POP  CX
           POP  BX
           POP  AX
           RET
CLEAR      ENDP
SPORTDIS   PROC  NEAR       
           PUSH  AX
           PUSH  BX
           PUSH  CX
           PUSH  DX
           PUSH  SI
           MOV  AH,0FH        
           INT  10H
           SUB  CH,CH
           MOV  CL,[SI]
           INC  SI
NEXT:      ADD  DH,[SI+2]
           ADD  DL,[SI+3]
           MOV  AH,2          
           INT  10H
           MOV  AL,[SI]
           MOV  BL,[SI+1]
           PUSH  CX
           MOV  CX,1
           MOV  AH,9        
           INT  10H
           POP  CX
           ADD  SI,4         
           LOOP  NEXT
           POP  SI
           POP  DX
           POP  CX
           POP  BX
           POP  AX
           RET
SPORTDIS   ENDP
CODE       ENDS
           END  MAIN
