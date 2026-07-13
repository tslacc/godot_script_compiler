#ifndef TMP_FILE_BUF
#define TMP_FILE_BUF
#include <stdio.h>
#include <stdlib.h>
#include "lib_bserial.c"
const int TFILE_BUF_INIT_SIZE = 512;
typedef struct temp_file {
	unsigned int total_size_allocated;
	unsigned int size_used;
	char *buffer;
} tfile;

tfile *ctr_tfile(){
	tfile *result = malloc(sizeof(tfile));
	result->buffer = malloc(TFILE_BUF_INIT_SIZE);
	result->total_size_allocated = TFILE_BUF_INIT_SIZE;
	result->size_used = 0;
	return(result);
}

void close_tfile(tfile *t){
	free(t->buffer);
	free(t);
	return;
}

const float TMP_FILE_BUFFER_SCALE_FACTOR = 1.5;

void write_bytes_to_tmp(tfile *t, void *input, int size){
	//printf("writing bytes to %u from %u size %u\n", t, input, size);
	//printf("Current buffer size is %u out of %u\n", t->size_used, t->total_size_allocated); 
	if(t->total_size_allocated < t->size_used + size){
		t->total_size_allocated*=TMP_FILE_BUFFER_SCALE_FACTOR;
		t->buffer = realloc(t->buffer, t->total_size_allocated);
	}
	char *set = (char *)input;
	while(size > 0){
		*(t->buffer+t->size_used) = *set;
		set++;
		t->size_used++;
		size--;
	}
	return;
}

void write_byte_to_tmp(tfile *t, char input){
	//printf("writing byte to %u val %u\n", t, input);
	//printf("Current buffer size is %u out of %u\n", t->size_used, t->total_size_allocated); 
	if(t->total_size_allocated < t->size_used + 1){
		t->total_size_allocated*=TMP_FILE_BUFFER_SCALE_FACTOR;
		t->buffer = realloc(t->buffer, t->total_size_allocated);
	}
	*(t->buffer+t->size_used) = input;
	t->size_used++;
	return;
}

void write_int_to_tmp(tfile *t, int input){
	char *bytes = int_to_bytes(input);
	write_bytes_to_tmp(t, bytes, sizeof(int));
	free(bytes);
	return;
}


void tfile_write_to_file(tfile *t, char *path){
	FILE *fp = fopen(path, "w");
	if(fp == NULL) return;
	int i = 0;
	while(i < t->size_used){
		putc(t->buffer[i], fp);
		i++;
	}
	return;
}
#endif