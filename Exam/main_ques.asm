;--------------------------------------------------------------------------------------
;                           microprocessor midterm  answer(for TA)
;       morse code is useful for sending information when you just have a little tool
;
;examination question
;       1.blink led with correspond string(just lower case letters)
;       2.you should put you string at pragram memory
;       3.just prepare morse code table for you to ckeck
;
;
;
;
;--------------------------------------------------------------------------------------

list p=18f4520
#include <p18f4520.inc>
;--------------------------------------------------------------------------------------
;configure area
;you can click configure bit to generate them


; CONFIG1H
  CONFIG  OSC = INTIO67         ; Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
  CONFIG  IESO = OFF            ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

; CONFIG2L
  CONFIG  PWRT = OFF            ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  BOREN = SBORDIS       ; Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
  CONFIG  BORV = 3              ; Brown Out Reset Voltage bits (Minimum setting)

; CONFIG2H
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  WDTPS = 1             ; Watchdog Timer Postscale Select bits (1:1)

; CONFIG3H
  CONFIG  CCP2MX = PORTC        ; CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
  CONFIG  PBADEN = ON           ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
  CONFIG  LPT1OSC = OFF         ; Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
  CONFIG  MCLRE = ON            ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

; CONFIG4L
  CONFIG  STVREN = ON           ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  CONFIG  XINST = OFF           ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

; CONFIG5L
  CONFIG  CP0 = OFF             ; Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
  CONFIG  CP1 = OFF             ; Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
  CONFIG  CP2 = OFF             ; Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
  CONFIG  CP3 = OFF             ; Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

; CONFIG5H
  CONFIG  CPB = OFF             ; Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
  CONFIG  CPD = OFF             ; Data EEPROM Code Protection bit (Data EEPROM not code-protected)

; CONFIG6L
  CONFIG  WRT0 = OFF            ; Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
  CONFIG  WRT1 = OFF            ; Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
  CONFIG  WRT2 = OFF            ; Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
  CONFIG  WRT3 = OFF            ; Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

; CONFIG6H
  CONFIG  WRTC = OFF            ; Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
  CONFIG  WRTB = OFF            ; Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
  CONFIG  WRTD = OFF            ; Data EEPROM Write Protection bit (Data EEPROM not write-protected)

; CONFIG7L
  CONFIG  EBTR0 = OFF           ; Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR1 = OFF           ; Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR2 = OFF           ; Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR3 = OFF           ; Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

; CONFIG7H
  CONFIG  EBTRB = OFF           ; Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

  
;--------------------------------------------------------------------------------------
;Define MACRO

;-------------------------------------------------


;morse code table location
#define morse_address  0x0300

;before locate program memory table 
;use it to write data to file register(to address bak_add)
;if table_value == 0 break;
ReadTable macro bak_add
    LOCAL   N_FIN
	LFSR    0,      bak_add
N_FIN   
    movf    TABLAT, 0
    movwf   POSTINC0
    TBLRD   +*
    bnz     N_FIN  
endm 

;Set up pragram memory table for reading at address table_add
SetTable  macro table_add
    ;TODO
endm

;Delay one unit 
Delay_unit macro 
    ;TODO
    
    
    endm
    
;Different input to blink led_led
;input == 0  blink 0 unit stop 3 unit
;input == 1  blink 1 unit stop 1 unit
;input == 2  blink 3 unit stop 1 unit
Blink_LED   macro    input
   ;TODO   
endm
   

;--------------------------------------------------------------------------------------
;main function
;   
    
RES_VECT  CODE    0x0000            ; processor reset vector
    GOTO    START                   ; go to beginning of program  

MAIN_PROG CODE                      ; let linker place main program

START
;init 
    call		led_init
    SetTable	??????
    ReadTable	??????              ;read string into file register
    
    LFSR    	0,0x00              ;read string 
    
loop
    ;TODO
    SetTable	??????
    movlw	    ??????              ;judeg what letter is 
    ?????       ??????
    bn		    final		        ;if ascii == 0 jp to final
    MULLW       ?                   ;make offset to read morse code of letter
    ;TODO
    
    
    
    ?????       TBLPTRL
loop2      
    TBLRD       *+ 
    Blink_LED	?????
    movf        ?????
    
    bnz		    loop2
    GOTO loop                          ; loop forever
final
    goto final
    
    
;--------------------------------------------------------------------------------------
;Define subr

;led_init
;1.set PORTA for output 
;2.clear value in LATA
led_init
    clrf    TRISA
    clrf    LATA
    return 

;--------------------------------------------------------------------------------------
;Define input string you should change string before \0   
inputstring code 
ques	    db "aaaaaa\0"

;morse code table 
;0 is use to align to 8 byte
;1 for dot
;2 for dash
;
morse_code code morse_address
    A_L     db 1,2,0,0,0,0,0,0
    B_L     db 2,1,1,1,0,0,0,0
    C_L     db 2,1,2,1,0,0,0,0
    D_L     db 2,1,1,0,0,0,0,0
    E_L     db 1,0,0,0,0,0,0,0
    F_L     db 1,1,2,1,0,0,0,0
    G_L     db 2,2,1,0,0,0,0,0
    H_L     db 1,1,1,1,0,0,0,0
    I_L     db 1,1,0,0,0,0,0,0
    J_L     db 1,2,2,2,0,0,0,0
    K_L     db 2,1,2,0,0,0,0,0
    L_L	    db 1,2,1,1,0,0,0,0
    M_L     db 2,2,0,0,0,0,0,0    
    N_L     db 2,1,0,0,0,0,0,0
    O_L     db 2,2,2,0,0,0,0,0
    P_L     db 1,2,2,1,0,0,0,0
    Q_L     db 2,2,1,2,0,0,0,0
    R_L     db 1,2,1,0,0,0,0,0
    S_L     db 1,1,1,0,0,0,0,0
    T_L     db 2,0,0,0,0,0,0,0
    U_L     db 1,1,2,0,0,0,0,0
    V_L     db 1,1,1,2,0,0,0,0
    W_L     db 1,2,2,0,0,0,0,0
    X_L     db 2,1,1,2,0,0,0,0
    Y_L     db 2,1,2,2,0,0,0,0
    Z_L     db 2,2,1,1,0,0,0,0
	END