section .text
bits 64
default rel

global arrMulAdd

;param: n - length of vector - long long int - RCX 
;		A - scalar - sd float - XMM1
;		X - vector 1 - address (sd float) - R8
;		Y - vector 2 - address (sd float) - R9
;		Z - resultant vector - address (sd float) - RBP+32
arrMulAdd:
	push rbp
	mov rbp, rsp
	add rbp, 16
	mov rsi, R8
	mov rdi, R9
	mov R15, [RBP+32]
	dec RCX
mul:
	MOVSD XMM2, [RSI+RCX*8]
	MOVSD XMM3, [RDI+RCX*8]
	MULSD XMM2, XMM1
	VADDSD XMM0, XMM2, XMM3
	MOVSD [R15+RCX*8], XMM0
	dec RCX
	CMP rcx, 0x0
	jge mul

pop rbp
ret