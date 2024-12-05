.data
zx: .word 0
zy: .word 0
.text
    .global drawJuliaSet


@ start from here
@r5 x, r6 y
drawJuliaSet:
    stmfd sp!,{r4-r12,lr}
    add lr,r0,pc           @ Demand-3
    mov r5, #0
    addeq r6,r5,r6         @ Demand
    subne r6,r6,r5         @ Demand
forloop1:
    mov r6, #0 @ y = 0
    forloop2:
        @ calcualte zx and zy
        bl calc_zx
        bl calc_zy
        mov r12,#255
        @ while loop
    while:
        @ check if quit
        ldr r7,=zx
        ldr r7,[r7,#0]
        ldr r8,=zy
        ldr r8,[r8]
        mul r7,r7,r7  @ Rd = Rm*Rs
        mul r8,r8,r8  @ Rd = Rm*Rs
        add r9,r7,r8
        mov r9, r9, lsl #4   @ operand2
        mov r9, r9, asr #2   @ operand2
        mov r9, r9, lsr #2   @ operand2

        ldr r10,=4000000
        cmp r9,r10
        bge end_while
        mov r10,#0
        cmp r12,r10
        ble end_while
        @ calculate temp
        ldr r7,=zx @ r7 = zx
        ldr r7,[r7]
        mul r7,r7,r7            @Rd=Rm*Rs
        sub r7,r7,r8
        @ r7 = (zx*zx-zy*zy)/1000
        stmfd sp!, {r0-r3,r12,lr}
        mov r0,r7
        ldr r1,=1000
        bl __aeabi_idiv
        mov r10,r0
        ldmfd sp!, {r0-r3,r12,lr}
        @ r7 = r7+zx
        add r10,r10,r0
        mov r10, r10, ror #16 @ operand2
        mov r10, r10, ror #16 @ operand2
        @zy
        ldr r7,=zx
        ldr r7,[r7]
        ldr r8,=zy
        ldr r8,[r8]
        mul r8,r7,r8
        lsl r8,#1
        @ zy = 2*zx*zy/1000 + cX
        stmfd sp!, {r0-r3,r12,lr}
        mov r0,r8
        ldr r1,=1000
        bl __aeabi_idiv
        mov r8,r0
        ldmfd sp!, {r0-r3,r12,lr}
        add r8,r8,r1
        ldr r9,=zy
        str r8,[r9]
        ldr r9, =zx
        str r10,[r9]

        sub r12,r12,#1
        b while
    end_while:
        @ color = ((i&0xff)<<8 | (i&0xff)
        @ color = (~color)&0xffff
        and r12,r12,#0xff
        orr r12,r12,r12,lsl #8
        ldr r11, =0xffff

        bic r12,r11,r12
        @ color = r12
        @ frame[y][x] = color
        ldr r7, =frame


        mul r8,r2,r6 @ width * y
        add r7,r7,r8, lsl #1 @ frame + y * width * 2
        add r7,r7,r5,lsl #1 @ frame + y * width * 2 + x * 2
        strh r12,[r7]


        add r6, #1
        cmp r6, r3
        blt forloop2
    add r5, #1
    cmp r5, r2
    blt forloop1

end:
    ldmfd sp!,{r4-r12,lr}
    mov pc,lr

calc_zx:
    stmfd sp!,{r7-r9,lr}
    mov r7, r5 @ x
    mov r8, r2 @ width
    ldr r9, =1500
    lsr r8, #1 @ (width>>1)
    sub r7,r7,r8 @ ( x - (width>>1))
    mul r9,r9,r7 @ 1500 * mid
    @div r9,r9,r8 @ all / r8
    stmfd sp!,{r0-r3,r12,lr}
    mov r0,r9
    mov r1,r8
    bl __aeabi_idiv

    mov r7,r0
    ldmfd sp!, {r0-r3,r12,lr}
    ldr r8,=zx
    str r7,[r8]
    ldmfd sp!,{r7-r9,lr}
    mov pc, lr

@ calculate y
calc_zy:
    stmfd sp!,{r7-r9,lr}
    mov r7, r6 @ y
    mov r8, r3 @ height
    ldr r9, =1000
    lsr r8, #1 @ (height>>1)
    sub r7,r7,r8 @ ( x - (height>>1))
    mul r9,r9,r7 @ 1000 * mid
    @div r9,r9,r8 @ all / r8
    stmfd sp!,{r0-r3,r12,lr}
    mov r0,r9
    mov r1,r8
    bl __aeabi_idiv

    mov r7,r0
    ldmfd sp!, {r0-r3,r12,lr}
    ldr r8,=zy

    str r7,[r8]
    ldmfd sp!,{r7-r9,lr}
    mov pc, lr
