#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("Welcome to my photo editor!\n");
  FILE *image_ptr = fopen("res/image.jpeg", "r");
  if (image_ptr == NULL) {
    printf("File could not be opened\n");
    return 1;
  }

  fseek(image_ptr, 0L, SEEK_END);
  long int size = ftell(image_ptr);
  rewind(image_ptr);

  unsigned char *image = malloc(size + 1);
  if (image == NULL) {
    fclose(image_ptr);
    return 1;
  }

  size_t bytes_read = fread(image, 1, size, image_ptr);
  if (bytes_read != size) {
    printf("Was not able to successfully read the file!\n");
    fclose(image_ptr);
    free(image);
    return 1;
  }

  fclose(image_ptr);

  printf("BYTES READ: %zu\tSIZE:%ld\n", bytes_read, size);

  /*
  for(int i = 0; i < size; i++) {
      printf("%02X ", image[i]);
  }
  printf("\n");
  */

  FILE *image_out_ptr = fopen("res/image_out.jpeg", "wa");
  if (image_out_ptr == NULL) {
    printf("Couldn't open new file!\n");
    return 1;
  }

  unsigned char start_of_scan[14] = {0xFF, 0xDA, 0x00, 0x0C, 0x03, 0x01, 0x00,
                                     0x02, 0x11, 0x03, 0x11, 0x00, 0x3f, 0x00};
  bool found = false;
  int i = 0;
  while (!found) {
    fprintf(image_out_ptr, "%c", image[i]);
    if (image[i] == 0xFF && image[i + 1] == 0xDA) {
      fprintf(image_out_ptr, "%c", image[++i]);
      found = true;
    }
    i++;
  }
  found = false;

  while (!found) {
    fprintf(image_out_ptr, "%c", image[i]);
    if (image[i] == 0x3F && image[i + 1] == 0x00) {
      fprintf(image_out_ptr, "%c", image[++i]);
      found = true;
    }
    i++;
  }

  for (; i < size - 2; i++) {
      fprintf(image_out_ptr, "%c", (image[i] * 0x05) % 0xFF);
  }

  fprintf(image_out_ptr, "%c", image[size - 2]);
  fprintf(image_out_ptr, "%c", image[size - 1]);
  fclose(image_out_ptr);

  free(image);
  return 0;
}
