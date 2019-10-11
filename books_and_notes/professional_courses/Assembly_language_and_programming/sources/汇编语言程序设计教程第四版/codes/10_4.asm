CRLF	MACRO			   
  	MOV  DL,0DH	
       	MOV  AH,02H
       	INT  21H		
       	MOV  DL,0AH
       	MOV  AH,02H
       	INT  21H
        ENDM
DATA    SEGMENT
DATA1   DB  33H,39H,31H,37H,34H,0
COUNT   EQU  $-DATA1
DATA2   DB  32H,35H,30H,38H,37H			
DATA    ENDS
STACK   SEGMENT  STACK 'STACK'
    	DB  20 DUP(?)
STACK   ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,DS:DATA,SS:STACK,ES:DATA
START:  MOV  AX,DATA
      	MOV   DS,AX
        MOV   SI,OFFSET DATA2   
        MOV   BX,COUNT   
        CALL  DISP
    	CRLF	
        MOV   SI,OFFSET DATA1	
        MOV   BX,COUNT 
        CALL   DISP
    	CRLF	
        MOV   DI,OFFSET DATA2
       	CALL  ADDA           
        MOV   SI,OFFSET DATA1
    	MOV   BX,COUNT		   
        CALL  DISP      
        CRLF
    	MOV   AX,4C00H
        INT   21H
DISP    PROC  NEAR		
DS1:    MOV   AH,02H
      	MOV   DL,[SI+BX-1]	
        INT   21H
    	DEC   BX		  
        JNZ   DS1
	RET
DISP	ENDP
ADDA	PROC  NEAR		
        MOV   DX,SI     
    	MOV   BP,DI
        MOV   BX,COUNT
AD1:    SUB   BYTE PTR [SI+BX-1],30H 
        SUB    BYTE PTR [DI+BX-1],30H
        DEC    BX            
        JNZ    AD1
        MOV   SI,DX
	MOV   DI,BP
        MOV   CX,COUNT
	CLC			
AD2:    MOV   AL,[SI]
	ADC   AL,[DI]
        AAA                
        MOV   [SI],AL    
	INC    SI   	
        INC    DI 
	LOOP  AD2
        MOV   SI,DX
	MOV   DI,BP
        MOV   BX,COUNT
AD3:    ADD   BYTE PTR [SI+BX-1],30H  
        ADD   BYTE PTR [DI+BX-1],30H
        DEC    BX            
	JNZ    AD3
        RET
ADDA	ENDP
CODE    ENDS
	END   START
