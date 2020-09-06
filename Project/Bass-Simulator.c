#include <xc.h>
// BEGIN CONFIG
#pragma config OSC = INTIO67   // Oscillator Selection bits (HS oscillator)
#pragma config WDT = OFF  // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRT = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON  // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF   // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)

#define _XTAL_FREQ 8000000
#define RESOLUTION 100
#define DEC 50000/RESOLUTION
#define CENTER 50
#define MAX 40
#define BASE_MAX MAX/4
#define BASE_RES_A 1
#define BASE_RES_B 9
#define BASE_RES_C 17
#define BASE_RES_D 25

PWM1_Duty(unsigned int duty);
hit();
wave();
weaken();

static int a, res_a, cnt_res_a, dec_a, cnt_dec_a, flag_a;
static int b, res_b, cnt_res_b, dec_b, cnt_dec_b, flag_b;
static int c, res_c, cnt_res_c, dec_c, cnt_dec_c, flag_c;
static int d, res_d, cnt_res_d, dec_d, cnt_dec_d, flag_d;
static int enhance, flag_hi, flag_lo;

void main(void) {
    //ADC
    ADCON1bits.VCFG1 = 0 ;  //setting vref-
    ADCON1bits.VCFG0 = 0 ;  //setting vref+
    ADCON1bits.PCFG  = 0b1011 ;  //Setting A/D Port Configuration Control
    ADCON0bits.CHS = 0b0000 ;    
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;
    TRISAbits.RA2 = 1;
    TRISAbits.RA3 = 1;
    ADCON2bits.ADFM = 0 ;    //setting left justified  11111111 11xxxxxx
    
    //setting acquisition time (ADCON2) ACQT 2:0
    //setting conversion time (ADCON2))
    ADCON2bits.ACQT = 0b010;
    ADCON2bits.ADCS = 0b111;
    ADCON0bits.ADON = 0; 
    
	//CCP
    TRISC = 0b00000000; //set CCP1 output (rc2))
    OSCCON = 0b01110000; //Tosc = 8MHz
    T2CON = 0b00000100; //enable timer2 ,prescale = 1
    CCP1CON = 0b00111100;//set PWM mode
    
    PR2 = 23;
    //TONE BUTTON
    TRISDbits.RD0 = 1;
    TRISDbits.RD1 = 1;
    TRISDbits.RD2 = 1;
    TRISDbits.RD3 = 1;
	//voice BUTTON
    TRISBbits.RB5 = 1;
    TRISBbits.RB4 = 1;
    //led output
    TRISCbits.RC6 = 0;  LATCbits.LC6=0;
    TRISCbits.RC7 = 0;  LATCbits.LC7=0;
           
    a=0;    res_a=BASE_RES_A;   cnt_res_a=0;    dec_a=DEC*res_a/BASE_MAX;   cnt_dec_a=0;    flag_a=0;
    b=0;    res_b=BASE_RES_B;   cnt_res_b=0;    dec_b=DEC*res_b/BASE_MAX;   cnt_dec_b=0;    flag_b=0;
    c=0;    res_c=BASE_RES_C;   cnt_res_c=0;    dec_c=DEC*res_c/BASE_MAX;   cnt_dec_c=0;    flag_c=0;
    d=0;    res_d=BASE_RES_D;   cnt_res_d=0;    dec_d=DEC*res_d/BASE_MAX;   cnt_dec_d=0;    flag_d=0;
    enhance=0, flag_hi=0, flag_lo=0;
    
	//a
	//res_a  	 何時轉換
	//cnt_res_a  轉換計數器(看有無達到轉換波的地方)
	//dec_a		 何時漸弱
	//cnt_dec_a  漸弱計數器(看有無達到漸弱的地方)
	//flag_a     判斷有無按鈕
	
    do {
        PWM1_Duty(CENTER+a+b+c+d);
        __delay_us(RESOLUTION);
        hit();
        wave();
        weaken();
    }while(1);
    return;
}

PWM1_Duty(unsigned int duty)
{
    CCP1CONbits.DC1B = duty & 0b11;
    CCPR1L = duty>>2;
}

hit(){
    if(PORTDbits.RD0==0 && flag_a==0){
        flag_a=1;
        ADCON0bits.CHS = 0b0000 ; //setting input channel (AN0)
        ADCON0bits.ADON = 1;	 //turn on ad module 
        ADCON0bits.GO = 1;		//開始偵測ADC
        while(ADCON0bits.GO);
        res_a = (ADRESH >> 5) + BASE_RES_A;  
        ADCON0bits.ADON = 0;	//turn OFF ad module 
        a = BASE_MAX + enhance * 6;
		dec_a=DEC*res_a/a;  
        cnt_res_a=0;  
        cnt_dec_a=0;
    }else if(PORTDbits.RD0==1 && flag_a==1){
        flag_a=0;
    }
    
    if(PORTDbits.RD1==0 && flag_b==0){
        flag_b=1;
        ADCON0bits.CHS = 0b0001 ;  
        ADCON0bits.ADON = 1;
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO);
        res_b = (ADRESH >> 5) + BASE_RES_B;
        ADCON0bits.ADON = 0;
        b = BASE_MAX + enhance * 3;
        dec_b=DEC*res_b/b; 
        cnt_res_b=0; 
        cnt_dec_b=0;
    }else if(PORTDbits.RD1==1 && flag_b==1){
        flag_b=0;
    }
    
    if(PORTDbits.RD2==0 && flag_c==0){
        flag_c=1;
        ADCON0bits.CHS = 0b0010 ;  
        ADCON0bits.ADON = 1;
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO);
        res_c = (ADRESH >> 5) + BASE_RES_C;
        ADCON0bits.ADON = 0;
        c = BASE_MAX - enhance * 3; 
        dec_c=DEC*res_c/c; 
        cnt_res_c=0; 
        cnt_dec_c=0;
    }else if(PORTDbits.RD2==1 && flag_c==1){
        flag_c=0;
    }
    
    if(PORTDbits.RD3==0 && flag_d==0){
        flag_d=1;
        ADCON0bits.CHS = 0b0011 ;  
        ADCON0bits.ADON = 1;
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO);
        res_d = (ADRESH >> 5) + BASE_RES_D;
        ADCON0bits.ADON = 0;
        d = BASE_MAX - enhance * 6; 
        dec_d=DEC*res_d/d; 
        cnt_res_d=0; 
        cnt_dec_d=0;
    }else if(PORTDbits.RD3==1 && flag_d==1){
        flag_d=0;
    }
    
    if(PORTBbits.RB5==0 && flag_hi==0 && enhance<1){
        flag_hi=1;  a=0;    b=0;    c=0;    d=0;
        LATCbits.LC6=0;
        if(++enhance==1) LATCbits.LC7=1;
    }else if(PORTBbits.RB5==1 && flag_hi==1){
        flag_hi=0;
    }
    
    if(PORTBbits.RB4==0 && flag_lo==0 && enhance>-1){
        flag_lo=1;  a=0;    b=0;    c=0;    d=0;
        LATCbits.LC7=0;
        if(--enhance==-1) LATCbits.LC6=1;
    }else if(PORTBbits.RB4==1 && flag_lo==1){
        flag_lo=0;
    }
}

wave(){
    if(++cnt_res_a==res_a) {cnt_res_a=0;    a=-a;}
    if(++cnt_res_b==res_b) {cnt_res_b=0;    b=-b;}
    if(++cnt_res_c==res_c) {cnt_res_c=0;    c=-c;}
    if(++cnt_res_d==res_d) {cnt_res_d=0;    d=-d;}  
}

weaken(){
    if(++cnt_dec_a==dec_a){
        cnt_dec_a = 0;
        if(a>0) --a;
        if(a<0) ++a;
    }
    if(++cnt_dec_b==dec_b){
        cnt_dec_b = 0;
        if(b>0) --b;
        if(b<0) ++b;
    }
    if(++cnt_dec_c==dec_c){
        cnt_dec_c = 0;
        if(c>0) --c;
        if(c<0) ++c;
    }
    if(++cnt_dec_d==dec_d){
        cnt_dec_d = 0;
        if(d>0) --d;
        if(d<0) ++d;
    } 
}