#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.c"
//Code referenced from https://en.cppreference.com/c/io/fopen
//No safety for commas inside apostrophes.
//Specifications: ints and longs are guaranteed (at least) 4 bytes, signed.
//This program accepts a CSV and outputs a compressed binary file.

//Reads at most count-1 characters from stream and writes them to str, IFF there is a comma or newline in that sequence.
//Returns str on success and NULL on failure.
//Exit program on fail to read properly.
char *fgets_comma(char *str, int count, FILE *stream){
	char *work = str;
	int c = 0;
	while ((c = fgetc(stream)) != EOF && count > 0) {
		if (c == ',' || c == '\n') {
			*work = '\0';
			return str;
		}
		*work = c;
		work++;
		count--;
	}
	if (c == EOF) {
		printf("End of file\n");
	} else {
		printf("Error when reading file, last char is %c %u\n",c,c);
	}
	return NULL;
}
char *fgets_newline_only(char *str, int count, FILE *stream){
	char *work = str;
	int c = 0;
	while ((c = fgetc(stream)) != EOF && count > 0) {
		if (c == '\n') {
			*work = '\0';
			return str;
		}
		*work = c;
		work++;
		count--;
	}
	if (c == EOF) {
		printf("End of file\n");
	} else {
		printf("Error when reading file, last char is %c %u\n",c,c);
	}
	return NULL;
}
//Read up to 32 bytes and return the next int delimited by a comma or newline.
int next_int(FILE *stream){
	const int READ_SIZE = 32;
	char *ptr = malloc(READ_SIZE*sizeof(int));
	ptr = fgets_comma(ptr, READ_SIZE, stream);
	int result = atoi(ptr);
	free(ptr);
	return result;
}

unsigned int tmp_file_size = 512;
unsigned int tmp_used_size = 0;
char *tmp_file;
char *tmp_file_ptr;
void init_tmp_file(){
	tmp_used_size = 0;
	tmp_file_size = 512;
	tmp_file = calloc(tmp_file_size, sizeof(char));
	tmp_file_ptr = tmp_file;
}
//write 'size' bytes to the array in tmp_file.
void write_bytes_to_tmp(void *input, int size){
	if(tmp_file_size < tmp_used_size + size){
		tmp_file_size*=2;
		tmp_file = realloc(tmp_file, tmp_file_size);
		tmp_file_ptr = tmp_file + tmp_used_size;
	}
	char *set = (char *)input;
	while(size > 0){
		tmp_file_ptr = set;
		++set;
		++tmp_file_ptr;
	}
	return;
}
void cleanup_tmp_file(){
	free(tmp_file);
	return;
}
int main(int argc, char * argv[])
{
	atexit(cleanup_tmp_file);
	if (argc > 1) {
		printf("Rcvd value %s\n", argv[1]);
	} else {
		printf("Rvcd no file\n");
		return 0;
	}
	FILE* fp;
	fp = fopen(argv[1],"r");
	if(fp==NULL){
		printf("Could not open file '%s'\n",argv[1]);
		return 0;
	}else{
		printf("Opened file '%s' with pointer %u\n", argv[1], fp);
	}
	int is_ok = EXIT_FAILURE;

    if (!fp)
    {
        perror("File opening failed");
        return is_ok;
    }

    int c; // note: int, not char, required to handle EOF

    while ((c = fgetc(fp)) != EOF) {// standard C I/O file reading loop
        if(c == '\n') printf("\\n");
        putchar(c);
	}
    if (ferror(fp))
        puts("I/O error when reading");
    else if (feof(fp))
    {
        puts("End of file is reached successfully");
        is_ok = EXIT_SUCCESS;
    }
    //Begin custom code
	rewind(fp);
	is_ok = EXIT_FAILURE;
	if(!fp){
		perror("File opening failed");
		return is_ok;
	}
	printf("Begin custom read\n");
	
	//Make a temporary file 
	init_tmp_file();
	
	while (!feof(fp)) {// standard C I/O file reading loop
 		//Initial state. 
 		//String holder
 		char *str = malloc(512*sizeof(char));
 		str = fgets_comma(str, 8, fp);
 		if (str == NULL) {
 			if (!feof(fp)) {
	 			perror("Failed to read operator. Closing file.\n");
	 			fclose(fp);
	 			return is_ok;
	 		}
	 		break;
 		}
 		
 		//Write str to file as an integer here.
		printf("Instruction %s = %u, ", str, parse_op_int(str));
		//TODO ADD TO TEMP FILE
		//TODO ERROR cast to pointer from integer of different size
		//only want the first byte, i.e. the number 0-255 as a single byte and write this to file
		
		
		//If this is any of the following instructions, get two integers and write to file.
 		if (strcmp(str, "ADD") == 0 || strcmp(str, "ADDI") == 0 || strcmp(str, "SUB") == 0 || strcmp(str, "SUBI") == 0 || strcmp(str, "MUL") == 0 || strcmp(str, "MULI") == 0 || strcmp(str, "DIV") == 0 || strcmp(str, "DIVI") == 0 || strcmp(str, "JE") == 0 || strcmp(str, "JNE") == 0 || strcmp(str, "JE") == 0 || strcmp(str, "JL") == 0 || strcmp(str, "JG") == 0)
		{
			int n = next_int(fp);
			printf("%d ",n);
			n = next_int(fp);
			printf("%d\n",n);
		} else if (strcmp(str, "JMP") == 0 || strcmp(str, "JMPR") == 0){
			int n = next_int(fp);
			printf("%d\n",n);
		} else if (strcmp(str, "WRITE") == 0 || strcmp(str, "READ") == 0){
			str = fgets_comma(str,512,fp);
			if (str == NULL){
				perror("Failed to read string operand.\n");
				fclose(fp);
				return is_ok;
			}
			printf("%s ",str);
			int n = next_int(fp);
			printf("%d\n",n);
		} else if (strcmp(str, "FUNC") == 0) {
			str = fgets_newline_only(str,512,fp);
			if (str == NULL){
				perror("Failed to read string operand.\n");
				fclose(fp);
				return is_ok;
			}
			printf("%s\n",str);
		}
	}
    fclose(fp);
    return is_ok;
}
//TODO Past line 146 change all prints to file writes.