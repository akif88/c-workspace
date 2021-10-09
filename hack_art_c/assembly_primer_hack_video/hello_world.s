# hello world assembly program

.data

HelloWorld:
	.ascii "Hello, world\n"

.text

.globl _start


_start:
	#write hello wold
	
	movl $4, %eax   		#syscall write()
	movl $1, %ebx    		#stdout
	movl $HelloWorld, %ecx		#buffer point string hello world
	movl $13, %edx			#string lenght
	int $0x80			#syscall are invoked by processes

	#exit program
	
	movl $1, %eax
	movl $0, %ebx
	int $0x80

