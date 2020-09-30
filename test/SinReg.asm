org #80

Data
byte 'A' ; Low-order byte
byte 'B' ; High-order byte
;
org #1000
Start
movlz Data,R0 ; R0 <- Address of Data
ld R0,R0 ; R0 <- BA (#4241)
;
swpb R0 ; R0 <- AB (#4142)

org #1000
movls #0F,R1 ; R1 <- #FF0F
sxt R1 ; R1 <- #000F
;
movlz #80,R1 ; R1 <- #0080
sxt R1 ; R1 <- #FF80

end Start