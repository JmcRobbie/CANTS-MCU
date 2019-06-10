
#if (__PICOSKY_HAVE_JP_JS__)
#define XJMP jp
#else
#define XJMP jpr
#endif

;Must be defined in this way, to prevent the jumps from being optimized into relative jumps
.section .supervisor_vect,"a",%progbits
.global	g_sup_vectors
.func g_sup_vectors

g_sup_vectors:
;***** Traps
	XJMP HW_RESET        ; hardware reset trap
	XJMP REG_FILE_FAULT  ; register file fault trap
	XJMP PROG_MEM_FAULT  ; program memory fault trap
	XJMP PROG_EXEC_FAULT ; program execution fault trap
	XJMP DATA_MEM_FAULT  ; data memory fault trap
	XJMP SW_TRAP         ; software trap
.endfunc

.section .supervisor,"ax"

;***** Hardware reset trap
HW_RESET:
	rst

;***** Register file fault trap
REG_FILE_FAULT:
	jpr REG_FILE_FAULT

;***** Program memory fault trap
PROG_MEM_FAULT:
	jpr PROG_MEM_FAULT

;***** Program execution fault trap
PROG_EXEC_FAULT:
	jpr PROG_EXEC_FAULT

;***** Data memory fault trap
DATA_MEM_FAULT:
	jpr DATA_MEM_FAULT

;***** Software trap
SW_TRAP:
	rtt
