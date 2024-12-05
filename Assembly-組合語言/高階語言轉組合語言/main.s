.data
    team_num: .asciz "team 05\n"
    name_mes: .asciz "Function1: Name\n"
    id_mes: .asciz	"Function2: ID\n"
    main_mes: .asciz "\nMain Function:\n"
    start_mes: .asciz "*****Print All*****\n"
    member_data: .asciz "%d  %s"
    total_mes: .asciz "\nID Summation = %d\n"
    end_mes: .asciz "*****End Print*****\n"
.text
    .global main
    .global id1
    .global id2
    .global id3
    .global name1
    .global name2
    .global name3
    .global idsum

main:
	stmfd	sp!, {lr}
	ldr	r0, =name_mes
	bl printf

	bl name

	ldr	r0, =id_mes
	bl printf

	bl id

	ldr r0, =main_mes
	bl printf

	ldr r0, =start_mes
	bl printf

    ldr r0, =team_num
    bl printf

    ldr r0, =member_data
    ldr r1, =id1
    ldr r1, [r1, #0]
    ldr r2, =name1
    bl printf

    ldr r0, =member_data
    ldr r1, =id2
    ldr r1, [r1, #0]
    ldr r2, =name2
    bl printf

    ldr r0, =member_data
    ldr r1, =id3
    ldr r1, [r1, #0]
    ldr r2, =name3
    bl printf

    ldr r0, =total_mes
    ldr r1, =idsum    @address of idsum
    ldr r1, [r1, #0]
    bl printf

    ldr r0, =end_mes
    bl printf

	ldmfd	sp!, {lr}
	mov pc, lr
