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
L1 EQU 0X24
L2 EQU 0X25
L3 EQU 0x26


;Delay one unit 
Delay_unit MACRO num1
    local LOOP1	;prevent compile error
    local LOOP2   
    local LOOP3
    MOVLW num1
    MOVWF L3
    LOOP3:
	MOVLW 0xC8 ;200
	MOVWF L2
    LOOP2:
	MOVLW 0xFA ;250             
	MOVWF L1  
    LOOP1:
	NOP
	NOP
	DECFSZ L1,1
	    GOTO LOOP1
	DECFSZ L2,1
	    GOTO LOOP2
	DECFSZ L3,1
	    GOTO LOOP3
	
    ENDM
    
;Different input to blink led_led
;input == 0  blink 0 unit stop 3 unit
;input == 1  blink 1 unit stop 1 unit
;input == 2  blink 3 unit stop 1 unit
Blink_LED   macro    input
    local BL0
    local BL1
    local BL2
    local GG
    MOVLW input
    ADDLW 0x01
    DCFSNZ WREG
    GOTO BL0
    DCFSNZ WREG
    GOTO BL1
    DCFSNZ WREG
    GOTO BL2

BL0:
    BCF LATA, 6
    Delay_unit 0x03
    GOTO GG
BL1:
    BSF LATA, 6
    Delay_unit 0x01
    BCF LATA, 6
    Delay_unit 0x01
    GOTO GG
BL2:
    BSF LATA, 6
    Delay_unit 0x03
    BCF LATA, 6
    Delay_unit 0x01
    GOTO GG
GG:
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
    GOTO	HEHE
    
    
;--------------------------------------------------------------------------------------
;Define subr

;led_init
;1.set PORTA for output 
;2.clear value in LATA
led_init
    clrf    TRISA
    clrf    LATA
    return 

;morse code table 
;0 is use to align to 8 byte
;1 for dot
;2 for dash
;
	
HEHE:
    Blink_LED 0x01
    Blink_LED 0x01
    Blink_LED 0x01
    Blink_LED 0x00
    
    Blink_LED 0x02
    Blink_LED 0x02
    Blink_LED 0x02
    Blink_LED 0x00
    
    Blink_LED 0x01
    Blink_LED 0x01
    Blink_LED 0x01
    
    Blink_LED 0x07
    
	END


