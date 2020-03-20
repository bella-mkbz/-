	UCSBFS SEGMENT CODE
	RSEG UCSBFS
	PUBLIC CSBFS
CSBFS:	mov R2,#8h    ;超声波发射的完整波形个数：共计四个
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
        djnz R2,here    
	RET
	END	
