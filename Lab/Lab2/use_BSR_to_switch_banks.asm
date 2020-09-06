LIST p=18f4520		
#include<p18f4520.inc>		

ORG     0x00 ; setting initial address
    
BSRTEST:    MOVLB 5 ; Pick Bank 5
	     MOVLW H'EE'
	     MOVWF H'520' ; Get Datum 1 using Banking mode into WREG
	     MOVLB 2 ; Pick Bank 2
	     MOVWF H'220',1 ; Store away using Banking mode
	     MOVLB 5 ; Pick Bank 5 again
	     MOVWF H'521',1 ; Get Datum 2 using Banking mode into WREG
	     MOVLB 2 ; Pick Bank 2 again
	     MOVWF H'221',1 ; Store away using Banking mode
	     END