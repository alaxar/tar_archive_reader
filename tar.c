/*

	writted by Alazar Demessie :) for educational purposes
	TAR FILE READER

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct tar_header
{
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
    char padding[355];
} __attribute__((packed));


// since the file size in stored in octal it must be chaned to decimal the below code will handle that for you.
unsigned int getsize(const char *in)
{
 
    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;
 
    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);
 
    return size;
}

int main(int argc, char *argv[]) {

	if(argc < 2)
		printf("Usage: ./tar <tar_archive.tar>\n");
		
	unsigned char *buffer = NULL;
	FILE *fp = fopen(argv[1], "rb");
	if(fp == NULL)	return -1;
	
	struct tar_header *header = malloc(sizeof(struct tar_header));
	memset(header, 0, sizeof(struct tar_header));
	unsigned i = 0;
	while(1) {
		// reading the header
		// check if we reached to the end of file.
		fread(header, 1, sizeof(struct tar_header), fp);

		if(header->filename[0] == '\0') return 0;
		
		printf("Filename: %s\n", header->filename);
		printf("FileSize: %d\n", getsize(header->size));
		printf("User ID: %s\n", header->uid);
		printf("Group ID: %s\n", header->gid);
		printf("Created data: %s\n", header->mtime);
		printf("******* DATA BELOW ********* \n\n");
		
		// alocate memory for buffer and clear its memory
		buffer = (unsigned char*)malloc(getsize(header->size));
		memset(buffer, '\0', getsize(header->size));
		
		// reading the content to buffer
		fread(buffer, 1, getsize(header->size), fp);
		printf("%s\n", buffer);
		printf("*********** END *************\n");
		// get the next header and content
		fseek(fp, 1024*(i+1), SEEK_SET);
		i++;
	}
	return 0;
}
