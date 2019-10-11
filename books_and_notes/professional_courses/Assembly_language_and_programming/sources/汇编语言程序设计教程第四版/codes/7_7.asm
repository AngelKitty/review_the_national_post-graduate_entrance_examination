SSEG          SEGMENT  STACK
              DW  100 DUP (?)
SSEG          ENDS
CSEG          SEGMENT
              ASSUME  CS:CSEG,SS:SSEG
PRINTO        PROC  FAR
              STI
              PUSH  ES
              PUSH  DI
              PUSH  DS
              PUSH  CX
              PUSH  DX
              PUSH  BX
              PUSH  AX
              MOV  DX,378H      
              OUT  DX,AL        
              MOV  DX,379H     
              SUB  BX,BX
WAIT:         IN  AL,DX           
              TEST  AL,80H      
              JNZ  ALL       
              JMP  WAIT     
ALL:          MOV  AL,0DH
              MOV  DX,37AH        
              OUT  DX,AL
              MOV  AL,0CH 
              OUT  DX,AL                                    
              POP  AX
              POP  BX
              POP  DX
              POP  CX
              POP  DS
              POP  DI
              POP  ES
              IRET
PRINTO        ENDP
MAIN:         MOV  AX,SSEG   
              MOV  SS,AX
              CLD
              MOV  AX,0
              MOV  ES,AX
              MOV  DI,4*70H
              MOV  AX,OFFSET PRINTO
              STOSW
              MOV  AX,SEG PRINTO
              STOSW
              MOV  AL,'A'
              INT  70H
              MOV  AH,4CH
              INT  21H
CSEG          ENDS
              END  MAIN
