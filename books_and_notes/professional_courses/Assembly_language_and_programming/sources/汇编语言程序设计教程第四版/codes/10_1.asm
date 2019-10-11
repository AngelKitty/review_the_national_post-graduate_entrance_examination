DATA	  SEGMENT
TABLE	  DW  0,1,8,27,64,125,216,343,512,729
XX	  DB	?
YY	  DW	?
DATA	  ENDS
STACK	  SEGMENT
	  DW	25 DUP(?)
STACK	  ENDS
CODE	  SEGMENT
	  ASSUME  CS:CODE,DS:DATA,SS:STACK
START:	  PUSH    DS 				
	  MOV     AX,0
	  PUSH    AX
   	  MOV     AX,DATA
	  MOV     DS,AX
	  MOV     BX,OFFSET TABLE
	  MOV     AH,0
          MOV     AL,XX
	  ADD     AL,AL		
	  ADD     BX,AX
	  MOV     AX,[BX]
	  MOV     YY,AX
	  RET
CODE	  ENDS
	  END	  START
