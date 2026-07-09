#ifndef OPCODES
#define OPCODES

typedef enum opcodes{
	NOP=0,
	//Arithmetic instructions
	ADD=1,	//dest = src1 + src2
	ADDI=2,	//dest = src1 + imm
	SUB=3,	//see add
	SUBI=4,
	MUL=5,	//dest = src1 * src2
	MULI=6,	//dest = src1 * imm
	DIV=7,	//see mul
	DIVI=8,
	//All jump instructions are absolute jumps by instruction position
	//All jump arguments shall be registers
	//Jump to arg0 if...
	JMP=9,	//true
	JE=10,	//arg1 == arg2
	JNE=11,	//arg1 != arg2
	JL=12,	//arg1 < arg2
	JG=13,	//arg1 > arg2
	JMPR=14,	//JMP to arg0's value
	//Write/read functions reference non-register memory by string reference
	//Implementation of what each string refers to is left to the program caller
	//Always refers to LOCATION first, then the register to read/write from.
	WRITE=15,	
	READ=16,	
	FUNC=17	//func(args...) where args is a single string.
} OPC;
const char *op_strings[] = {
	"NOP",
	"ADD",
	"ADDI",
	"SUB",
	"SUBI",
	"MUL",
	"MULI",
	"DIV",
	"DIVI",
	"JMP",
	"JE",
	"JNE",
	"JL",
	"JG",
	"JMPR",
	"WRITE",
	"READ",
	"FUNC"
	};
int parse_op_int(char *input){
	for(int i = 0; i < (sizeof(op_strings)/sizeof(const char *)); i++){
		if(strcmp(op_strings[i], input) == 0) return i;
	}
	return 0;
}
#endif