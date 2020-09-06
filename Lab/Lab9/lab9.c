#define TMR2PRESCALE 16
#define _XTAL_FREQ 500000

#include <xc.h>
// BEGIN CONFIG
#pragma config OSC = INTIO67   // Oscillator Selection bits (HS oscillator)
#pragma config WDT = OFF  // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRT = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON  // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF   // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)

//END CONFIG
PWM1_Init(long setDuty)
{
  PR2 = setDuty; 
}

PWM1_Duty(unsigned int duty)
{
//set duty to CCPR1L , CCP1X(CCP1CON:5) and CCP1Y(CCP1CON:4)
    CCPR1L = duty >> 2;
    CCP1X = duty & 0b10;
    CCP1Y = duty & 0b01;
}

PWM1_Start()
{
  //set CCP1CON
    CCP1CON = 0b00001100; //b'11001100
  //set timer2 on
    T2CON = 0b00000111;
  //set rc2 output
    TRISC = 0;
    
  if (TMR2PRESCALE == 1)
  {
    T2CKPS0 = 0;
    T2CKPS1 = 0;
  }
  else if(TMR2PRESCALE == 4)
  {  
    T2CKPS0 = 1;
    T2CKPS1 = 0;
  }
  else if (TMR2PRESCALE == 16)
  {
    T2CKPS0 = 1;
    T2CKPS1 = 1;
  }
 
}

void main()
{
  //set LATD to input
  TRISD=0xFF;
  
  // set FOSC to 500000 => TOSC = 1/500000
  OSCCONbits.IRCF = 0b011; 
    
  // Period = (PR2 + 1) * 4 * TOSC * 16 = 20 / 1000
  // PR2 = 155.25
  PWM1_Init(155);
  PWM1_Start();
  
  // Duty cycle = TOSC * 16 * (CCPR1L : CCPCON<5:4>)
  // 1/500000 * 16 * x = 0.5/1000   =>  x = 15.625
  // 1/500000 * 16 * x = 2.4/1000   =>  x = 75
  int i=15;
  do{
    if(PORTDbits.RD0==0 && i<75){ 
      i=i+1;
    }
    PWM1_Duty(i);
    __delay_ms(50);
  }while(1);
  
}