#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Welcome to my photo editor!\n");
    FILE *image_ptr = fopen("res/image.jpeg", "r");
    if(image_ptr == NULL) {
        printf("File could not be opened\n");
        return 1;
    }

    fseek(image_ptr, 0L, SEEK_END);
    long int size = ftell(image_ptr);
    rewind(image_ptr);

    unsigned char *image = malloc(size + 1);
    if(image == NULL) {
        fclose(image_ptr);
        return 1;
    }

    size_t bytes_read = fread(image, 1, size, image_ptr);
    if(bytes_read != size) {
        printf("Was not able to successfully read the file!\n");
        fclose(image_ptr);
        free(image);
        return 1;
    }
    
    fclose(image_ptr);

    printf("BYTES READ: %zu\tSIZE:%ld\n", bytes_read, size);

    for(int i = 0; i < size; i++) {
        printf("%02X ", image[i]);
    }
    printf("\n");


    free(image);
    return 0;
}

