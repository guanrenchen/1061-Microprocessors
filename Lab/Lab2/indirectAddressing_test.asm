LIST p=18f4520		
#include<p18f4520.inc>		

ORG     0x00 ; setting initial address
    
INDRECT:   MOVLW 0XDD
    	     LFSR 1,0X530	; Similarily ,FSR1 (Pointer 1) set to h'530'
	     LFSR 0,0X220	; Pointer FSR0 (Pointer 0) set to h'220'
	     MOVWF POSTINC1	; Get Datum 1 from File h'530'& FSR1++
	     MOVWF POSTINC0	; Store away at File h'220' & FSR0++
	     MOVWF INDF1	; Get Datum 1 from File h'531'
	     MOVWF INDF0	; and store away at File h'221'
	     NOP
	     END