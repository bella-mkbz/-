ORG 0000H   
SJMP START       
ORG 0030H 
  
START:
LOOP1: LCALL DISPLAY         
       LCALL DELAY1  
	   SETB P1.4
	   SETB P1.5
	   CLR P1.6
	   CLR P1.0	 
       CLR P1.2      
       SETB P1.2
       CLR P1.2
LOOP2: CPL P1.3
       JNB P1.1, LOOP2
       SETB P1.0       
       MOV R0, P3      
       LCALL BTODZ        
       SJMP LOOP1
	   
	   
DISPLAY: MOV R1, #3DH
      MOV R2, #0FDH
      MOV R3, #3
LOOP: MOV A, R2
      MOV P2, A 
	  RL A   
      MOV R2, A
      MOV A, @R1    
	  INC R1
      MOV DPTR, #TAB
      MOVC A, @A+ DPTR
	  MOV P0, A
	  LCALL DELAY
	  DJNZ R3, LOOP
	  RET
 
DELAY: MOV R5, #02H
DEL:   MOV R4, #0FFH
DEL1:	DJNZ R4, DEL1
		DJNZ R5, DEL
		RET

DELAY1: MOV R7, #01 
D11:    MOV R6, #80H 
D12:    LCALL DISPLAY         
DJNZ    R6, D12        
DJNZ    R7, D11         
RET

BTODZ: MOV A, R0         
       MOV B, #100         
       DIV AB         
       MOV 3DH, A    ;百位    
          
       MOV A, B    
	   MOV B, #10     
       DIV AB        
       MOV 3EH, A     ;十位    
       MOV 3FH, B      ;个位   
       RET
	   
TAB: DB 3FH, 06H, 5BH, 4FH, 66H, 6DH, 7DH, 07H, 7FH, 6FH, 77H, 7CH, 39H, 5EH, 79H, 71H, 00H    
	   

END