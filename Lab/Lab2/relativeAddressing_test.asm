LIST p=18f4520		
#include<p18f4520.inc>		

ORG     0x00 ; setting initial address

S2:	MOVLW 0X00
	MOVWF PCLATH
	MOVLW 0X04
	ADDWF PCL
	NOP
	NOP
	NOP
	NOP
	END