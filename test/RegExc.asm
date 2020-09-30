	org	#80		; Data section
Data	word	#0
;
	org	#1000		; Code section
Start	movlz	Data,R0		; R0 <- address of Data (#0080)
	ldr	R0,$0,R1	; R1 <- mem[R0]
;
Loop	cmp	#8,R1		; Check R0 = 8
	cex	ne,#2,#0	; Do 2 instructions if NE; skip 2 if EQ
; 
	add	#1,R1		; R1 <- R1 + 1
	bra	Loop		; Repeat
;
	str	R1,R0,$0	; mem[R0] <- R1
    swap R1,R3     ;
    MOV R0,R5      ;
Done				; Use as breakpoint
;
	end	Start		; S9 record get address of Start (#1000)