SPL1 ;set priority
; set all bit first 
sec ;clear carry bit
sen ;clear negative bit
sez ;clear zero bit
sev ;clear overflow bit
; Then clear it
clc ;clear carry bit
cln ;clear negative bit
clz ;clear zero bit
clv ;clear overflow bit
;Brand Instructions
beq loop
bz loop
bne loop
bne loop
beg loop
blt loop
bc loop
bnc loop
bn loop

loop

bl subr1
push R0 ;stack push arg
pull R0 ;stack pull arg

jump $1000 ;

subr1 
org $80 
ret ;return from subr1

; clear byte in SP
clr.b SP
clr SP
clr.w SP