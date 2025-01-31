#include <stdio.h>

int main() {
    printf("Welcome to my photo editor!\n");
    FILE *image_ptr = fopen("image.jpeg", "r");
    if(image_ptr == NULL) {
        printf("File could not be opened\n");
        return -1;
    }

    fseek(image_ptr, 0L, SEEK_END);
    long int size = ftell(image_ptr);
    fclose(image_ptr);

    printf("%ld\n", size);
    return 0;
}

