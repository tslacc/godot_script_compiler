#include <stdio.h>
#include <stdlib.h>
//Code referenced from https://en.cppreference.com/c/io/fopen
int main(int argc, char * argv[])
{

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

    fclose(fp);
    return is_ok;
}