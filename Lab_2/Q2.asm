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
	LDR R0,=NUM1
	LDR R1,=NUM2
	LDR R2,=RES
	MOV R3,#4
	
UP	LDR R4,[R0],#4
	LDR R5,[R1],#4
	ADCS R6,R5,R4
	STR R6,[R2],#4
	SUB R3,#1
	TEQ R3,#0
	BNE UP
	
NUM1 DCD 0x12340000, 0xFFEE0000, 0x12349078, 0x67000000
NUM2 DCD 0x67900000, 0x56780000, 0x45670000, 0xFFEE0000
	AREA mydata, DATA, READWRITE
RES DCD 0x0
	END 