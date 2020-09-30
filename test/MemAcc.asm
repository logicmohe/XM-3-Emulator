org #1000
Data1 bss #2
;
org #2000
Start
;
; R0 <- address of Data1
movl Data1,R0 ; Low-byte of R0 <- #00
movh Data1,R0 ; High-byte of R0 <- #10
;
; Write byte #F0 to Data1[0]
movlz #F0,R1 ; R1 <- #F0
str.b R1,R0,#0 ; mem[R0 + #0] <- R1
;
; Write byte #0F to Data1[1]
movlz #0F,R1 ; R1 <- $0F
str.b R1,R0,#1 ; mem[R0 + #1] <- R1
;
; Retrieve word from Data1[0]
ldr.w R0,#0,R2 ; R2 <- mem[R0 + #0]
end Start