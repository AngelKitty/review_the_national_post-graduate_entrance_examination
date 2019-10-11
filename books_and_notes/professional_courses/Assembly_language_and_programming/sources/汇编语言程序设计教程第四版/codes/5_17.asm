    DATA      SEGMENT
    ENUM      DB  14H  
    INUM      DW  2800H     
    DATA      ENDS
    STACK     SEGMENT  STACK
              DW  100 DUP (?)
    STACK     ENDS
    CODE      SEGMENT
              ASSUME  CS:CODE,SS:STACK,DS:DATA
    TIMEDL    PROC  FAR
              PUSH  DS
              MOV  AX,0
              PUSH  AX
              MOV  AX,DATA
              MOV  DS,AX
              MOV  BX,OFFSET  ENUM
              MOV  DL,[BX]   
              INC  BX
    DELAY:    MOV  CX,[BX]   
    WDY:      LOOP  WDY
              DEC  DL  
              JNZ  DELAY
              RET
    TIMEDL    ENDP
    CODE      ENDS
              END  TIMEDL
