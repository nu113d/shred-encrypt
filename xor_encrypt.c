#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void encrypt(char* buffer, size_t buffer_size,int key){
    for(size_t i = 0; i < buffer_size; i++){
        buffer[i] ^= key;
        key += 8;
    }
}

int process_file(const char* filename, int key){
    FILE* inputf;
    unsigned char* buffer;
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
    buffer = (unsigned char *)malloc(buffer_size); 
    fread(buffer, filelen, 1, inputf);
    fclose(inputf);
    encrypt(buffer, buffer_size, key);
    
    //reopen stream to write
    FILE* outputf = fopen(filename, "wb");
    fwrite(buffer, buffer_size, 1, outputf);
    fclose(outputf);
    free(buffer);

    return 0;

}

int main(int argc, char** argv){
   
    int key;
    int opt;
    // Process command-line options
    while ((opt = getopt(argc, argv, "k:")) != -1) {
        switch (opt) {
            case 'k':
                key = atoi(optarg);
                key = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s -k <key> <file1> [<file2> ...]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (!key) {
        fprintf(stderr, "Usage: %s -k <key> <file1> [<file2> ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Ensure at least one file is provided
    if (optind >= argc) {
        fprintf(stderr, "Expected at least one file after options\n");
        return EXIT_FAILURE;
    }

    // Process each file provided
    for (int i = optind; i < argc; i++) {
        if (process_file(argv[i], key) != EXIT_SUCCESS) {
            fprintf(stderr, "Failed to process file: %s\n", argv[i]);
        }
    }

    return 0;
}