DATA      SEGMENT
BUFF      DB  128   
          DB  ?
          DB 128 DUP(0)
MESS0     DB 'PLEASE INPUT STRING:$'  
MESS1 	  DB 'LENGTH=$' 
MESS2     DB 'X=$'      
MESS3     DB 'Y=$'   
DATA      ENDS
STACK     SEGMENT  STACK
          DW  100 DUP(?)
STACK     ENDS
CODE      SEGMENT
          ASSUME  CS:CODE,SS:STACK,DS:DATA
START:    MOV  AX,DATA
          MOV  DS,AX
          MOV  DX,OFFSET MESS0   
          CALL  DISPLAY
          MOV  DX,OFFSET BUFF
          MOV  AH,0AH      
          INT  21H
          CALL  NEWLINE   
          MOV  BH,0
          MOV  BL,0
          MOV  CL,BUFF+1	
          MOV  CH,0
          JCXZ  COK                  
          MOV  SI,OFFSET BUFF+2   
AGAIN:    MOV  AL,[SI]      
          INC  SI
          CMP  AL,'0'    
          JB  NEXT
          CMP  AL,'9'
          JA  NODEC
          INC  BH            
          JMP  SHORT NEXT
NODEC:    OR  AL,20H     
          CMP  AL,'a'
          JB  NEXT
          CMP  AL,'z'
          JA  NEXT
          INC  BL         
NEXT:     LOOP  AGAIN     
COK:      MOV  DX,OFFSET MESS1
          CALL  DISPLAY
          MOV  AL,BUFF+1   
          XOR  AH,AH
          CALL  DISDEC   
          CALL  NEWLINE
          MOV  DX,OFFSET MESS2
          CALL  DISPLAY
          MOV  AL,BH
          XOR  AH,AH
          CALL  DISDEC     
          CALL  NEWLINE
          MOV  DX,OFFSET MESS3
          CALL  DISPLAY
          MOV  AL,BL
          XOR  AH,AH
          CALL  DISDEC    
          CALL  NEWLINE
          MOV  AH,4CH
          INT  21H
DISDEC    PROC NEAR     
          MOV  CX,3   
          MOV  DL,10
DISP1:    DIV  DL
          XCHG  AH,AL
          ADD  AL,'0'
          PUSH  AX        
          XCHG  AH,AL
          MOV  AH,0
          LOOP  DISP1
          MOV  CX,3
DISP2:    POP  DX  
          MOV  AH,2
          INT  21H
          LOOP  DISP2
          RET
          DISDEC  ENDP
DISPLAY   PROC  NEAR    
          MOV  AH,9
          INT  21H
          RET
DISPLAY   ENDP
NEWLINE   PROC NEAR    
          MOV  AH,2
          MOV  DL,0DH
          INT  21H
          MOV  DL,0AH
          INT  21H
          RET
NEWLINE   ENDP
CODE      ENDS
          END  START
