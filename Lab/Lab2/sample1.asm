LIST p=18f4520		
#include<p18f4520.inc>		

ORG     0x00 ; setting initial address

		
S1: MOVLW 0X99		;
    ;MOVWF LATD,0	; case 1
    ;MOVWF LATD,1	; case 2
    ;MOVWF LATD		; case 3 
    NOP
    END