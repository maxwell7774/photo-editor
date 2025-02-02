#include "image.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main() {
  printf("Welcome to my photo editor!\n");
  jpeg_t *image_ptr = open_image("res/image.jpeg");
  if (image_ptr == NULL) {
    perror("Couldn't open the image");
    return 1;
  }
  bool save_successful = save_image(image_ptr, "new_image");
    if(!save_successful) {
        perror("Couldn't save the image");
        return 1;
    }

  return 0;
}
