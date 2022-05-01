##########################################################################################################################
# Created by: Quiring, Harrison
#		hquiring
#		November 16, 2020
#
# Assignment: Lab 3: ASCII-risks (Asterisks)
#		CSE-12, Computer Systems and Assembly Language
#		UC Santa Cruz, Fall 2020
#
# Description: This program asks the user for a height of a pyramid and then prints a pyramid of said height using asterisks and tabs.
#
# Notes: This program is intended to be run from the MARS IDE.
##########################################################################################################################
# PSEUDOCODE
# Have user enter height
# Set columncount=1
# Loop entire program with each count representing which row the program is on until the counter equals the height
# Have a loop inside main loop that repeatedly prints tabs for height-mainloopcounter times
# Print the value of columncount and print a tab 
# Increment columncount by 1
# Have a loop inside main loop that repeatedly prints an asterisk followed by a tab for 2*mainloopcounter-3 times
# If it is the first row do not print the value of columncount again and simply go to a new line
# Else print the value of columncount again, go to a new line, and increment columncount by 1
.data
	Prompt: .asciiz "Enter the height of the pattern: "
	Tab: .asciiz "	"
	Asterisk: .asciiz "*"
	Newline: .asciiz "\n"
.text
	#REGISTER USAGE
	#$t0: User input for height
	#$t1: Counter that tracks which number to print for each column
	#$t2: Loop counter that encompasses the entire program
	#$t3: Holder for true/false, also functions as the loop counter for both the tabbing loop and asterisk loop
	#$t4: Represents the max times the asterisk loop should run, effectively holds the equation 2X-3
	#$t5: Holder for true/false

	#Prompt the user to enter a number
	li $v0, 4
	la $a0, Prompt
	syscall
	#User input
	li $v0, 5
	syscall
	#Sets the max height= $t0
	move $t0, $v0
	#Sets $t1 equal to the columncount, starts at 1
	addi $t1, $t1, 1
	#Sets $t2 as the row counter used to track the number of times to loop, starts at 1
	addi $t2, $t2, 1
	
	Main_rowloop:
		#Exit condition, checking to see if $t2>$t0, uses $t3 as true/false, can temp reuse $t3 elsewhere
		slt $t3, $t0, $t2
		beq $t3, 1, ExitMainLoop
		#Continues the main loop if not exited
	
		#Another loop that focuses solely on the initial tabbing for the pyramid, uses $t3 as counter starting at 0, max is height-rowcounter
		move $t3, $zero
		sub $t5, $t0, $t2 
		Tabbing_loop:
			#Exit condition
			beq $t3, $t5, ExitTabLoop
	
			#Outputs a tab
			li $v0, 4
			la $a0, Tab
			syscall
	
			#End of tab loop and increments tab counter by 1
			add $t3, $t3, 1
			j Tabbing_loop
		ExitTabLoop:
	
		#Outputs 1st column number in this row with a tab afterwards, then increments the columncount($t1) by 1
		li $v0, 1
		add $a0, $t1, $zero
		syscall
		li $v0, 4
		la $a0, Tab
		syscall
		addi $t1, $t1, 1
	
		#Another loop that outputs the correct number of asterisks, uses $t3 as a counter starting at 0
		move $t3, $zero
		#Uses $t4 as the max number of times to loop, represents the equation $t4= 2 * $t2 - 3
		add $t4, $t2, $t2
		addi $t4, $t4, -3
		Asterisk_loop:
			#Exit condition, needs to be able to exit on greater than in addition to equal to due to $t4 possibly starting out negative, uses $t5 as true/false, can temp use $t5 elsewhere
			slt $t5, $t4, $t3
			beq $t5, 1, ExitAsteriskLoop
			beq $t4, $t3, ExitAsteriskLoop
			
			#Prints an asterisk following by a tab
			li $v0, 4
			la $a0, Asterisk
			syscall
			li $v0, 4
			la $a0, Tab
			syscall
			
			#End of asterisk loop and increments asterisk counter by 1
			add $t3, $t3, 1
			j Asterisk_loop
		ExitAsteriskLoop:
	
		#If statement that outputs another column number if necessary then increments column count($t1) by 1 and always indents a new line
		beq $t2, 1 ElseOfIf
			li $v0, 1
			add $a0, $t1, $zero
			syscall
			li $v0, 4
			la $a0, Newline
			syscall
			addi $t1, $t1, 1
			j EndOfIf
		ElseOfIf:
			li $v0, 4
			la $a0, Newline
			syscall	
		EndOfIf:
		#End of main loop and increments row counter by 1
		addi $t2, $t2, 1
	j Main_rowloop
	ExitMainLoop:
	li $v0, 10
	syscall