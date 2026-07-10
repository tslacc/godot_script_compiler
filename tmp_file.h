#ifndef TMP_FILE_BUF
#define TMP_FILE_BUF
#include <stdio.h>
#include <stdlib.h>

typedef struct temp_file {
	unsigned int total_size_allocated;
	unsigned int size_used;
	char *buffer;
} tfile;

tfile *ctr_tfile(){
	tfile *result = malloc(sizeof(tfile));
	return(result);
}

void close_tfile(tfile *t){
	free(t->buffer);
	free(t);
	return;
}

const float TMP_FILE_BUFFER_SCALE_FACTOR = 1.5;

void write_bytes_to_tmp(tfile *t, void *input, int size){
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
	if(t->total_size_allocated < t->size_used + 1){
		t->total_size_allocated*=TMP_FILE_BUFFER_SCALE_FACTOR;
		t->buffer = realloc(t->buffer, t->total_size_allocated);
	}
	*(t->buffer+t->size_used) = input;
	t->size_used++;
	return;
}


#endif