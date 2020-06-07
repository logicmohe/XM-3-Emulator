org #0
;
CLKCSR bss 1
;
org #1000
;
;
movlz CLKCSR,R0

start org $80

bne start

ld R0,R1
bit $4,R1
; BC is not used here