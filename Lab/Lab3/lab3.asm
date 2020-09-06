LIST p=18f4520		
#include<p18f4520.inc>		
; CONFIG1H
  CONFIG  OSC = INTIO67              ; Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
; CONFIG2H
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

org 	0x00	
	
Initial:	clrf WREG
	clrf LATA
	clrf LATB
	clrf LATC
	clrf LATD
; ***********************************
start:	MOVLW b'10110101'
	ANDLW b'01111100'
	XORLW b'11111111'
	MOVWF LATD
	MOVLW b'10010110'
	IORLW b'01101001'
	XORLW b'11111111'
	MOVWF LATC
	clrf WREG
	INCF WREG
	ADDWF LATA, 1
	BNOV 0x1C
	GOTO Rotate
	NOP
	NOP
	NOP
	GOTO Initial
Rotate:	MOVLW 0x8F
	MOVWF LATB
	RLCF LATB, 0
end

