#include <xc.h>

#define _XTAL_FREQ 10000000 //What ever speed you set your internal(or)external oscillator

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF

void main(int argc, char** argv) {
    
    TRISBbits.RB0 = 1; //set RB0 as Input
    TRISDbits.RD3 = 0;
    
    INTCONbits.GIE = 1;
    RCONbits.IPEN = 0x01;

    INTCONbits.INT0E = 1; //enable Interrupt 0 (RB0 as interrupt)
    INTCON2bits.INTEDG0 = 0; //cause interrupt at falling edge
    INTCONbits.INT0F = 0; //reset interrupt flag

    LATD = 0x00;
    //ei();     // This is like fliping the master switch to enable interrupt

    while(1) //infinite loop
    {
      //actually we have to put the processor in sleep which i will cover
      //  in later tutorials
    }


}

void interrupt CheckButtonPressed()
{
    //check if the interrupt is caused by the pin RB0
    if(INTCONbits.INT0E==1 && INTCONbits.INT0F == 1)
    {
        LATD = 0xFF;
        __delay_ms(200);
        LATD = 0x00;
        INTCONbits.INT0F = 0;
    }

}