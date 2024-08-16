#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void encrypt(char* buffer, size_t buffer_size){
    unsigned char* rand_buffer = (unsigned char *)malloc(buffer_size);
    arc4random_buf(rand_buffer, buffer_size);

    for(size_t i = 0; i < buffer_size; i++){
        buffer[i] ^= rand_buffer[i];
        
    }
    free(rand_buffer);
}

int process_file(const char* filename){
    FILE* inputf;
    unsigned char* file_buffer;
    long filelen;
    size_t buffer_size;

    //open input stream
    inputf = fopen(filename, "rb");
    if(inputf == NULL){
        printf("Failed to open file\n");
        return 1;
    }
     //get size of the file
    fseek(inputf, 0, SEEK_END);      
    filelen = ftell(inputf);     
    if (filelen < 0) {
        printf("ftell failed\n");
        fclose(inputf);
        return 1;
    }  
    buffer_size = filelen *sizeof(unsigned char);     
    rewind(inputf);        

    //store it in buffer
    file_buffer = (unsigned char *)malloc(buffer_size); 
    fread(file_buffer, filelen, 1, inputf);
    fclose(inputf);
    encrypt(file_buffer, buffer_size);
    
    //reopen stream to write
    FILE* outputf = fopen(filename, "wb");
    fwrite(file_buffer, buffer_size, 1, outputf);
    fclose(outputf);
    free(file_buffer);

    return 0;

}

int main(int argc, char** argv){
   
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [<file2> ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i <= argc - 1; i++) {
        if (process_file(argv[i]) != EXIT_SUCCESS) {
            fprintf(stderr, "Failed to process file: %s\n", argv[i]);
        }
    }

    return 0;
}