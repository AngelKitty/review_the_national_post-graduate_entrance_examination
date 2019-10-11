CONVERT   MACRO       
LOCAL     INP1,INP2,INP3,INP4     
          CMP  DL,0                 
          JE  INP4
          CMP  DL,'9'      
          JBE  INP1
          SUB  DL,7       
INP1:     AND  DL,0FH     
          CMP  DH,0            
          JE  INP3
          CMP  DH,'9'
          JBE  INP2
          SUB  DH,7
INP2:     PUSH  CX
          MOV  CL,4
          SHL  DH,CL          
          POP  CX
          OR  DL,DH             
INP3:     MOV  [BX],DL      
          INC  BX                   
          INC  CX
INP4:
          ENDM
INPUT     PROC                     
          PUSH  AX
          PUSH  DX
          XOR  CX,CX
A1:       XOR  DX,DX
A2:       MOV  AH,1             
          INT  21H
A3:       CMP  AL,0DH            
          JE  A9
          CMP  AL,' '    
          JE  A8
          CMP  AL,','
          JE  A8
          CMP  AL,08H       
          JE  A7
          CMP  AL,'0'        
          JB  A7
          CMP  AL,'f'    
          JA  A7
          CMP  AL,'a'
          JB  A4
          SUB  AL,20H        
          JMP  A5
A4:       CMP  AL,'F'      
          JA  A7
          CMP  AL,'A'          
          JAE  A5
          CMP  AL,'9'         
          JA  A7
A5:       CMP  DL,0             
          JNE  A6
          MOV  DL,AL             
          JMP  A2                
A6:       CMP  DH,0              
          JNE  A7
          MOV  DH,DL     
          MOV  DL,AL
          JMP  A2
A7:       MOV  DL,7       
          MOV  AH,2
          INT  21H
          MOV  DL,'?'
          INT  21H
          JMP  A1                
A8:       CONVERT        
          JMP   A1               
A9:       CONVERT     
          POP  DX
          POP  AX
          RET
INPUT     ENDP
ALDISP    PROC         
          PUSH  AX
          PUSH  DX
          PUSH  AX
          MOV  DL,AL
          SHR  DL,1        
          SHR  DL,1
          SHR  DL,1
          SHR  DL,1
          OR  DL,30H      
          CMP  DL,39H       
          JBE  D1
          ADD  DL,7
D1:       MOV  AH,2
          INT  21H
          POP  DX
          AND  DL,0FH        
          OR  DL,30H
          CMP  DL,39H 
          JBE  D2
          ADD  DL,7
D2:       MOV  AH,2
          INT  21H
          POP  DX
          POP  AX
          RET 
ALDISP    ENDP
SORTING   PROC           
          CMP  CX,0        
          JE  R4
          CMP  CX,1
          JE  R4
          PUSH  AX
          PUSH  DX
          PUSH  SI
          MOV  SI,BX   
          DEC  CX
R1:       MOV  DX,CX       
          MOV  BX,SI
R2:       MOV  AL,[BX] 
          CMP  AL,[BX+1]         
          JNA  R3
          XCHG  AL,[BX+1]        
          MOV  [BX],AL
R3:       INC  BX
          DEC  DX
          JNZ  R2
          LOOP  R1
          POP  SI
          POP  DX
          POP  AX
R4:       RET
SORTING   ENDP
