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
	LDR R13,=0x10001000
	MOV R1,#5
	MOV R2,#1
	BL FACT
	
STOP B STOP
	
FACT
	PUSH{R14}
	PUSH{R1}
	
	SUB R1,#1
	CMP R1,#1
	BEQ down
	BL FACT
	
down
	POP{R1}
	POP{R14}
	MUL R2,R1
	
	BX LR

	END