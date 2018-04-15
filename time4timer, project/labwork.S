  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
  
  	
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
.global delay

	
delay: 
	addi $t1, $0, 4450
while:
	beq $a0, $0, escape
	nop
	sub $a0, $a0, 1
	addi $t0, $0, 0
for:
	beq $t0, $t1, while
	nop
	addi $t0, $t0, 1
	j for
escape:
	jr $ra
	nop

	