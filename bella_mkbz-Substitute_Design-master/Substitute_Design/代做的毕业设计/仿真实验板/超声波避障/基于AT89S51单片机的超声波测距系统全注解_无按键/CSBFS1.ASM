	UCSBFS1 SEGMENT CODE
	RSEG UCSBFS1
	PUBLIC CSBFS1
CSBFS:	mov R1,#8h    ;超声波发射的完整波形个数：共计四个
 here:	cpl p2.7       ;输出40kHz方波        
        nop  
        nop              
        nop
        nop  
        nop              
        nop
        nop  
        nop              
        nop                                                                  
        djnz R1,here    
	RET
	END	
