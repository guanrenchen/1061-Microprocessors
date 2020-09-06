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

char ch;
int i=0, x=1;
int cnt=0, max_cnt=5;

int cnt2=0, max_cnt2=20;


int auto_flag=0;
int value;
int past[4];
int past_cnt=0, max_past_cnt=4;

//END CONFIG
int measure(){
    if(++past_cnt>=max_past_cnt) past_cnt=0;
    ADCON0bits.ADON = 1; 
    ADCON0bits.GO = 1;		//開始偵測ADC
    while(ADCON0bits.GO);
    past[past_cnt] = ADRESH<<8;
    past[past_cnt] += ADRESL;
    ADCON0bits.ADON = 0; 
    return past[past_cnt];
}

PWM1_Duty(unsigned int duty)
{
    CCP1CONbits.DC1B = duty & 0b11;
    CCPR1L = duty>>2;
}
void Myusartwrite(char a)
{
    TXREG = a;
    return ;
}

void write_value(){
    int z = 0x200;
    while(z>0){
        if((value & z) != 0)
            Myusartwrite('1');
        else
            Myusartwrite('0');
        while(TXIF==0);
        z = z >> 1;
    }
    Myusartwrite('\t');
    while(TXIF==0);
}

void MyusartRead()
{
    //TODO
    ch = RCREG; 
    if(ch=='p'){
        ADCON0bits.ADON = 1; 
        ADCON0bits.GO = 1;		//開始偵測ADC
        while(ADCON0bits.GO);
        value = ADRESH<<8;
        value += ADRESL;
        ADCON0bits.ADON = 0; 
        write_value();
    }
    if(ch=='a'){
        if(auto_flag==0) auto_flag=1;
        else             auto_flag=0;
    }
    return ;
}

void interrupt  Hi_ISR(void){
    if(RCIF){
        MyusartRead();
        RCIF = 0;
    }
    
    if(TMR1IE && TMR1IF){
        PWM1_Duty(i*63);
        i += x;
        if(i == 16) x = -1;
        if(i == 0)  x = 1;
        TMR1 = 65535-6250;
        TMR1IF=0;
        
        if(auto_flag==1){
            if(++cnt==max_cnt){
                cnt=0;
                measure();
            }
            if(++cnt2==max_cnt2){
                cnt2=0;
                value = 0;
                for(int j=0; j<4; ++j)
                    value += past[j];
                value >>= 2;
                write_value();
            }
        }
        
    }  
   
    
    
}

void PWM_INIT(){
	/************PWM************/
    TRISCbits.RC2=0;
//    TRISC = 0b00000000; //set CCP1 output (rc2))
//    OSCCON = 0b01110000; //Tosc = 8MHz
    T2CON = 0b00000100; //enable timer2 ,prescale = 1
    CCP1CON = 0b00111100;//set PWM mode
    PR2 = 0b11111111;
    
}

void TIMER1_INIT(){
    RCONbits.IPEN=0x01;
    T1CON=0b10010101; 
    PIE1bits.TMR1IE=1;
    IPR1bits.TMR1IP=1;
    PIR1bits.TMR1IF=0;
    INTCONbits.GIE=1;
    TMR1 = 65535-6250;
}

void ADC_INIT(){
    /************ADC************/
    ADCON1bits.VCFG1 = 0 ;  //setting vref-
    ADCON1bits.VCFG0 = 0 ;  //setting vref+
    ADCON1bits.PCFG  = 0b1110 ;  //Setting A/D Port Configuration Control
    ADCON0bits.CHS = 0b0000 ;    
    TRISAbits.RA0 = 1;
    ADCON2bits.ADFM = 1 ; 
    //setting acquisition time (ADCON2) ACQT 2:0
    //setting conversion time (ADCON2))
    ADCON2bits.ACQT = 0b010;
    ADCON2bits.ADCS = 0b011;
    ADCON0bits.ADON = 1; 
}

void UART_INIT(){
  /************UART**************/  
    RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 1;     //enable low priority interrupt
    /*portC pin6/7 is mult with usart setting their tris*/    
    TRISCbits.TRISC6 = 1;            // Setting by data sheet 
    TRISCbits.TRISC7 = 1;            //  
    BAUDCONbits.BRG16 = 0;          //Read Baud rate table
    TXSTAbits.BRGH = 0;
    SPBRG = 12;      
   //   Serial enable
    TXSTAbits.SYNC = 0;             //choose the async moode
    RCSTAbits.SPEN = 1;              //open serial port
    // Interrutp Setting 
    //setting TX/RX
    TXSTAbits.TXEN = 1;             //Enable Tx
    RCSTAbits.CREN = 1;             //Enable Rx
    PIR1bits.TXIF = 0;
    PIR1bits.RCIF = 0;
    //setting TX/RX interrupt
    PIE1bits.TXIE = 1;              //Tx interrupt
    IPR1bits.TXIP = 1;              //Setting Tx as high/low priority interrupt
    PIE1bits.RCIE = 1;              //Rx interrupt
    IPR1bits.RCIP = 1;              //Setting Rc as high/low priority interrupt
}

void main()
{
    PWM_INIT();
    TIMER1_INIT();
    ADC_INIT();
    UART_INIT();
    
  // Period = (PR2 + 1) * 4 * TOSC * 16 = 20 / 1000
  // PR2 = 155.25
  // Duty cycle = TOSC * 16 * (CCPR1L : CCPCON<5:4>)
  // 1/500000 * 16 * x = 0.5/1000   =>  x = 15.625
  // 1/500000 * 16 * x = 2.4/1000   =>  x = 75
    while(1);
}

