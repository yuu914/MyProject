@team-05
.data
    strt:   .asciz "*****Print Name*****\n"
    str1:   .asciz "Team 05\n"
    @name1:  .asciz "Lee Hung Yun\n"
    @name2:  .asciz "Lin Hui Yu\n"
    @name3:  .asciz "Chen Chia Yin\n"
    str5:   .asciz "*****End Print*****\n"
    cl:     .asciz "\n"

.text
    .global name
    @.global name1
    @.global name2
    @.global name3

name:
    stmfd sp!,{lr}
    ldr r0,=strt
    bl printf
    ldr r0,=str1
    bl printf
    ldr r0,=name1
    bl printf
    ldr r0,=name2
    bl printf
    ldr r0,=name3
    bl printf
    ldr r0,=str5
    bl printf

    mov r1, r13
    mov r2, r14
    mov r13, #0
    mov r14, r15
    adds r15, r14, r13
    mov r13, r1
    mov r14, r2

    ldmfd sp!,{lr}
    mov pc, lr
