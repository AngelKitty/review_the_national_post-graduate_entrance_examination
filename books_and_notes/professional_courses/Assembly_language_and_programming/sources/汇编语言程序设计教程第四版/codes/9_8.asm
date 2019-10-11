CODE    SEGMENT
        ASSUME  CS:CODE
START:  MOV  AH,0
        INT  16H       
        CMP  AL,32H      
        JC  START
        CMP  AL,39H     
        JNC  START
        AND  AL,0FH
        MOV  CL,AL     
        MOV  CH,0
A1:     MOV  DL,0DH
        MOV  AH,2
        INT  21H
        MOV  DL,0AH
        MOV  AH,2
        INT  21H
        INC  CH           
        CMP  CL,CH   
        JNC  A2
        MOV  AH,4CH
        INT  21H
A2:     XOR  CL,CL     
A3:     MOV  DL,'*' 
        MOV  AH,2
        INT  21H
        INC  CL
        CMP  CL,CH     
        JNZ  A3 
        JMP  A1       
CODE    ENDS
        END  START
