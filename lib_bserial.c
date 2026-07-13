#ifndef LIB_BSERIAL
#define LIB_BSERIAL
//Converts a given integer into bytes
//Output shall be in little-endian
char *int_to_bytes(int input){
	union un {
		int b_int;
		char bytes[sizeof(int)];
	};
	union un val;
	val.b_int = input;
	char *result = malloc(sizeof(int));
	memcpy(result, val.bytes, sizeof(int));
	return result;
}
#endif