/*
;==========================================
; Autore: Salvatore Pappalardo
; Matricola: 281621
; github: jspkay
; Data:   20 Marzo 2021
;==========================================
*/


int strlen(char *str){
	int i = 0;
	while( *(str+i) != 0){
		i += 1;
	}
	return i;
}

void strcpy(char *dst, char *src, int limit){
	int i = 0;
	do{
		*dst = *src;
		dst++;
		src++;
		i++;
		if(i >= limit-2) break;
	}while(*src != 0);
	
	*dst = 0;
}

// 	    	rdi			rsi			rdx
int write(int fd, char *string, int len){
	int r;

	// "movq %0, %%rsi \n\t" 	// buffer -> rsi is already OK
	// "movq %1, %%rdi \n\t" 	// file descriptor -> rdi already ok
	// "movq %2, %%rdx \n\t" 	// lenght -> rdx already ok
		
	__asm__ volatile(
		"movq $1, %rax \n\t" 	// syscall write
		"syscall \n\t"
	);

	// No return statement needed: rax is set by the syscall
}


int main(int argc, char *argv){ 
	int r;
	char message[25] = "Hello sysprog!\n";
	int l = strlen(message);

	if(argv == 0x0) // se la stringa non ha un puntatore valido
		argc = 1; 	// stampo il messaggio di default
	
	if(argc > 2){
		strcpy(message, "ERROR: Only one argument!\n", 25);
		write(0, message, strlen(message));
		return -2;
	}

	if(argc == 2){
		strcpy(message, argv, 25);
		l = strlen(message);
	}
	r = write(0, message, l);

	r = r<0 ? -1 : 0;
	
	return r;
}

//  		rdi
void exit(int ret){	
	__asm__("movq $60, %rax \n\t" 	// exit syscall
			//"movq %0, %%rdi \n\t" 	// exit code
			"syscall \n\t"
	);
}

void _start(){
	int argc; 
	char *string; // 2 vars * 8 bytes
	// total 16 bytes of local variables
	// plus 8 of bp

	__asm__( 
		"mov 24(%%rsp), %0 \n\t" // 16+8 = 24
		"mov 40(%%rsp), %1  \n\t" // get only second argument
		: "=r" (argc), "=r" (string)
	);
	
	exit(main(argc, string));
}
