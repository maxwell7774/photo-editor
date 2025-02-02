#include "jpeg.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("-------------------------------\n");
  printf("| Welcome to my photo editor! |\n");
  printf("-------------------------------\n\n");

  jpeg_t *jpeg = open_jpeg("res/image.jpeg");
  if (jpeg == NULL) {
    perror("Couldn't open the image");
    return 1;
  }

  bool save_successful = save_jpeg(jpeg, "new_image");
  if (!save_successful) {
    free_jpeg(jpeg);
    perror("Couldn't save the image");
    return 1;
  }

  free_jpeg(jpeg);
  return 0;
}
