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
	LDR R0,=ARR
	MOV R1,#10
	LDR R2,=ELE
	LDR R8,=RES
	LDR R3,[R2]; R3 has the element to be searched stored in it 
	MOV R5,#0
	MOV R6,#0x11111111
	MOV R7,0xFFFFFFFF
	
UP	LDR R4,[R0,R5]
	CMP R4,R3
	BEQ present
	ADD R5,#4
	SUBS R1,#1
	BNE UP
npresent
	STR R7,[R8]
	B STOP
present
	STR R6,[R8]
STOP 
	B STOP
ARR DCD 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10
ELE DCD 0x11
	AREA mydata, DATA, READWRITE
RES DCD 0x0
	END