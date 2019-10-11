DATA      SEGMENT
HOUR	  DB  0	
MINUTE    DB  0
SECOND	  DB  0	
HRCH	  DB  0		 
HRCL	  DB  0	
MNCH	  DB  0
MNCL	  DB  0
SCCH	  DB  0
SCCL 	  DB  0	
MSG       DB  'Please Input Initialized time:$'
BUFFER    DB  7,9 DUP(?)
WINDOW    DB  32 DUP(20H),'****************',32 DUP(20H)
          DB  32 DUP(20H),'*              *',32 DUP(20H)
          DB  32 DUP(20H),'*              *',32 DUP(20H)
          DB  32 DUP(20H),'*              *',32 DUP(20H)
          DB  32 DUP(20H),'****************',32 DUP(20H)
CONT      EQU   $-WINDOW
DATA      ENDS
CODE      SEGMENT
          ASSUME  CS:CODE,DS:DATA,ES:DATA
START:    MOV  AX,DATA
          MOV  DS,AX
          MOV  ES,AX
          MOV  AH,0	
          MOV  AL,2
          INT  10H
          CLEAR 	           
          LEA  DX,MSG     
          MOV  AH,9
          INT  21H
          LEA  DX,BUFFER    
          MOV  AH,0AH
          INT  21H
          LEA  DI,BUFFER+2	
          CALL  ASCTOBCD 		
          MOV  AL,HRCH
          MOV  CL,4
          ROL   AL,CL	
          ADD   AL,HRCL	   
          MOV   HOUR,AL	
          MOV  AL,MNCH
          MOV  CL,4
          ROL  AL,CL
          ADD  AL,MNCL
          MOV  MINUTE,AL		
          MOV  AL,SCCH
          MOV  CL,4
          ROL  AL,CL
          ADD  AL,SCCL
          MOV  SECOND,AL	     
          CLEAR			
          DSPWIN  CONT,WINDOW     
CYCLE:    CALL  DELAY1S     
          MOV  AL,SECOND
          ADD  AL,1
          DAA
          CMP  AL,59        
          JLE  NEXT1
          MOV  AL,0
          ADD  MINUTE,1    
NEXT1:    MOV  SECOND,AL
          MOV  AL,MINUTE
          ADD  AL,0
          DAA
          CMP  AL,59        
          JLE   NEXT2
          MOV  SECOND,0
          MOV  AL,0
          ADD  HOUR,1      
NEXT2:    MOV  MINUTE,AL
          MOV  AL,HOUR
          ADD  AL,0
          DAA
          CMP   AL,23    
          JLE   NEXT3
          MOV   AL,0		
          MOV   MINUTE,0
          MOV   SECOND,0
NEXT3:    MOV   HOUR,AL
          SCRN	SECOND,0C2CH	 
          SCRN  ':',0C2AH 	
          SCRN  MINUTE,0C29H	 
          SCRN  ':',0C27H 
          SCRN  HOUR,0C26H    
          CURSR  1900H         
          MOV   AH,06H
          MOV   DL,0FFH
          INT   21H	
          CMP  AL,27		
          JE EXIT
          JMP  CYCLE
EXIT:     MOV  AH,4CH
          INT  21H
CODE      ENDS
          END  START
