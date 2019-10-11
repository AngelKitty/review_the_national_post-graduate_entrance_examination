STACK    SEGMENT
         DW  64 DUP(?)
STACK    ENDS
CSEG     SEGMENT
         ASSUME  CS:CSEG,SS:STACK
START    PROC  FAR
         PUSH  DS
         XOR  AX,AX
         PUSH  AX
         MOV  CX,0005H 
         MOV  SI,3500H
         MOV  DI,3600H 
A1:     MOV  BL,0FFH
         MOV  AL,[SI]
         CMP  AL,3AH  
         JNB  A2  
         SUB  AL,30H
         JB  A2
         MOV  BL,AL 
A2:     MOV  AL,BL
         MOV  [DI],AL
         INC  SI
         INC  DI   
         LOOP  A1
         RET
START    ENDP
CSEG     ENDS
         END  START
