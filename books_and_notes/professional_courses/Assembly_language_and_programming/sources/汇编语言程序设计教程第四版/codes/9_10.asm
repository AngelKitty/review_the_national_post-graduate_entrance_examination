DATA        SEGMENT
STRING      DB  'Type"M"for 80*25 monochrome',0DH,0AH  
            DB  'Type"H"for 640*200 b & w',0DH,0AH,'$'
DATA        ENDS
CODE        SEGMENT
            ASSUME  CS:CODE,DS:DATA
START:      MOV  AX,DATA
            MOV  DS,AX
            MOV  DX,OFFSET STRING
            MOV  AH,9
            INT  21H
            MOV  AH,1     
            INT  21H
            CMP  AL,'M'     
            JE  M80
            CMP  AL,'H'     
            JE  H640
            JMP  START        
M80:        MOV  AL,2         
            MOV  AH,0
            INT  10H
            JMP  OVER
H640:       MOV  AL,6      
            MOV  AH,0
            INT  10H
OVER:       MOV  AH,4CH
            INT  21H
CODE        ENDS
            END  START
