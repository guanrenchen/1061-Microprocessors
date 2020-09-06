#include <xc.h>
// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
// CONFIG4L
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

int count=0;
void main(void) {
    RCONbits.IPEN=0x01;
    T1CON=0b10010101; 
    // 1 : 16-bit
    // 0
    // 0 : prescale value == 1:2
    // 1
    // 0
    // 1
    // 0
    // 1
   
    PIE1bits.TMR1IE=1;
    IPR1bits.TMR1IP=1;
    PIR1bits.TMR1IF=0;
    
    INTCONbits.GIE=1;
    
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    
    LATD = 0;
    // TMR1 = 3035
    TMR1H = 0x0B;
    TMR1L = 0xDD;
    
    while(1);
}

void interrupt tc_int(void)     // High priority interrupt
{
    if(TMR1IE && TMR1IF){
        PORTDbits.RD0 = ~PORTDbits.RD0;
        if(++count==2){
            PORTDbits.RD1 = ~PORTDbits.RD1;
            count = 0;
        }
        TMR1H = 0x0B;
        TMR1L = 0xDD;
        TMR1IF=0;
    }  
    
}