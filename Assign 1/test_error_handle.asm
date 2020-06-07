SIZE equ #26
CAP_A equ 'A'
CAP_Z equ 'Z'
; Start of data area
org $80
Base bss SIZE ; Reserve SIZE bytes
; Start of code area
org $1000
Start movlz CAP_A,R0 ; R0 = 'A'
clr CAP_Z,R1 ; R1 = 'Z'
clr.b Base,R2 ; R2 = Base (Base address for characters
;
Loop
st.b R0,R2+ ; [R2] = R0; R2 = R2 + 1
cmp.b R0,R1 ; R0 = R1 ('Z')
push 
; ERROR here, missing the operand
BN
; ERROR here, missing the operand
add.b #1,R0 ; No: R0 = R0 + 1 (next ASCII char)
beq Loop ; Repeat loop

Done bra Done ; Infinite loop to "stop" the program
;
end Start