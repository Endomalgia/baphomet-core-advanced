#include "util.h"

char* fileGetString(char* filepath) {
	FILE* fptr;
	char* string;
	long size;

	// Open the file
	if (!(fptr = fopen(filepath, "r"))) {
		THROW("Failed to open file %s", filepath);
	}
	
	//Seek towards the end
	if (fseek(fptr,0L,SEEK_END) != 0) {
		fclose(fptr);
		THROW("Failed to seek in file %s",filepath);
	}
	
	// Report the length and reset the position of the file pointer
	size = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);	

	// Allocate a buffer
	string = malloc((size+1) * sizeof(char));
	
	// Read the file into the buffer
	fread(string, sizeof(char), size, fptr);		
	string[size++] = '\0';
	
	// Check if the process worked
	if (ferror(fptr)) {
		fclose(fptr);
		THROW("Failed to read file %s",filepath);
	}

	// Cleanup
	fclose(fptr);
	return string;
}