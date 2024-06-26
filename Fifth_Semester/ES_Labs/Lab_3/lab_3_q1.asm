	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x40001000
	DCD Reset_Handler
	ALIGN
	AREA mycode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0,=hex
	LDR R1,=ascii
	LDR R2,[R0]
	
	AND R3,R2,#0xF0
	LSR R3,#4
	CMP R3,#0x09
	BLE UP
	ADD R3,#0x07
	
UP	ADD R3,#0x30
	
	AND R4,R2,#0x0F
	CMP R4,#0x09
	BLE UP2
	ADD R4,#0x07
	
UP2	ADD R4,#0x30
	
	STR R3,[R1]
	STR R4,[R1,#2]
STOP B STOP
hex DCB 0x1A
	AREA mydata, DATA, READWRITE
ascii DCB 0x0		
	END