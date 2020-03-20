#include "ADC0832.h"

#define pulse0832() _nop_();_nop_();CLK_0832=1;_nop_();_nop_();CLK_0832=0 

unsigned char read0832()  
{  
    unsigned char i, ch = 0, ch1 = 0;  
    CS_0832=0;        
      
    DI_0832=1;  
     
    pulse0832();      
      
    DI_0832=1;  
    pulse0832();     
      
    DI_0832=1;  
    pulse0832();     
      
     
    DI_0832=1;  
      
     
    for(i = 0; i < 8; ++i) {  
        pulse0832();  
        ch <<= 1;  
        if(DI_0832==1)  
            ch |= 0x01;  
    }  
      
     
    for(i = 0; i < 8; ++i) {  
        ch1 >>= 1;  
        if(DI_0832==1)  
            ch1 |= 0x80;  
        pulse0832();  
    }  
      
    CS_0832=1;       
    return (ch==ch1) ? ch : 0;        
}  