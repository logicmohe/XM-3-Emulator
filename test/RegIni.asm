CapA equ 'A' ; Equated character constant
Limit equ $255 ; Equated decimal constant $255
NUL equ #0 ; Equated hex constant #0
;
org #800
Start
; To assign R0 the value #2010:
movl #10,R0
movh #20,R0
;
; To assign CapA to the lower byte of R3:
movl CapA,R3
MOVLZ $0,R3
MOVLS #FF,R4
ORG #1000
; Data
Array bss $16 ; Reserve 16 bytes
; Code
ORG #2000
MOVL Array,R0 ; Least-significant byte of Array assigned to R0
MOVH Array,R0 ; Most-significant byte of Array assigned to R0
ORG #80
end Start
