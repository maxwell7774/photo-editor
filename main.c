#include "image.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main() {
  printf("Welcome to my photo editor!\n");
  jpeg_t *image_ptr = open_image("res/image.jpeg");
  if (image_ptr == NULL) {
    perror("Couldn't open the image;");
    return 1;
  }
  save_image(image_ptr, "n.e.w_image");

  /*
  for(int i = 0; i < size; i++) {
      printf("%02X ", image[i]);
  }
  printf("\n");

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

  */
  return 0;
}
