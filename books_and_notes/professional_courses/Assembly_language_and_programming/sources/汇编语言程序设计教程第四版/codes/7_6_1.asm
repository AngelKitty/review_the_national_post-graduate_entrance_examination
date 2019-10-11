DATA      SEGMENT
MESS      DB  'This is an example about soft interrupt. $'
ERR       DB  0AH,0DH,'ERROR!$'
DATA      ENDS
          EXTRN  INT4AH:FAR
CODE      SEGMENT
          ASSUME  CS:CODE,DS:DATA
BG£º      MOV  AX,SEG INT4AH   
          MOV  DS,AX
          MOV  DX,OFFSET INT4AH
          MOV  AX,254AH          
          INT  21H
          MOV  AX,DATA
          MOV  DS,AX
          MOV  AX,OFFSET  MESS
          INT  4AH
          CMP  AH,0FFH        
          JE  ER         
          MOV  AH,4CH
          INT  21H
ER£º      LEA  DX,ERR     
          MOV  AH,9
          INT  21H 
          MOV  AH,4CH
          INT  21H
CODE      ENDS
          END  BG

