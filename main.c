#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.c"
#include "tmp_file.h"
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
	/*
	if (c == EOF) {
		printf("End of file\n");
	} else {
		printf("Error when reading file, last char is %c %u\n",c,c);
	}
	*/
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

int main(int argc, char * argv[])
{
	if (argc > 1) {
		printf("Opening file \"%s\"\n", argv[1]);
	} else {
		printf("No file input\n");
		return 0;
	}
	FILE* fp;
	fp = fopen(argv[1],"r");
	if(fp==NULL){
		printf("Could not open file '%s'\n",argv[1]);
		return 0;
	}else{
		printf("Opened file \"%s\" p= %u\n", argv[1], fp);
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
        puts("End of file reached successfully");
        is_ok = EXIT_SUCCESS;
    }
    //Begin custom code
	rewind(fp);
	is_ok = EXIT_FAILURE;
	if(!fp){
		perror("File opening failed");
		return is_ok;
	}
	
	//Make a temporary file 
	tfile *tmp_file = ctr_tfile();
	char *str = malloc(512*sizeof(char));	
	
	while (!feof(fp)) {// standard C I/O file reading loop
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
		write_byte_to_tmp(tmp_file, parse_op_int(str));
		
		//If this is any of the following instructions, get two integers and write to file.
 		if (strcmp(str, "ADD") == 0 || strcmp(str, "ADDI") == 0 || strcmp(str, "SUB") == 0 || strcmp(str, "SUBI") == 0 || strcmp(str, "MUL") == 0 || strcmp(str, "MULI") == 0 || strcmp(str, "DIV") == 0 || strcmp(str, "DIVI") == 0 || strcmp(str, "JE") == 0 || strcmp(str, "JNE") == 0 || strcmp(str, "JE") == 0 || strcmp(str, "JL") == 0 || strcmp(str, "JG") == 0)
		{
			int n = next_int(fp);
			printf("%d ",n);
			write_int_to_tmp(tmp_file, n);
			n = next_int(fp);
			printf("%d",n);
			write_int_to_tmp(tmp_file, n);
		} else if (strcmp(str, "JMP") == 0 || strcmp(str, "JMPR") == 0){
			int n = next_int(fp);
			printf("%d\n",n);
			write_int_to_tmp(tmp_file, n);
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
			write_bytes_to_file(tmp_file, str, strlen(str)); 
			printf("%s\n",str);
		}
		printf("\n");
	}
    fclose(fp);
    free(str);
    tfile_write_to_file(tmp_file, "test.output");
    close_tfile(tmp_file);
    return is_ok;
}