#include "stc12_adc.h"

/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks

void Delay(unsigned int n)
{
    unsigned int x;
    while (n--)
    {
        x = 5000;
        while (x--);
    }
}

void InitADC()
{
    P1ASF = 0x01;                   //Open First channels ADC function, P1.0
    ADC_RES = 0;                    //Clear previous result
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay(2);                       //ADC power-on and delay
}

unsigned char GetADCResult()
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START;
    _nop_();                        //Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//Wait complete flag
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    //if you want show 10-bit result, uncomment next line
    //    ADC_RESL                  // ADC low 2-bit result
	
    return ADC_RES;                 //Return  ADC high 8-bit result
}
