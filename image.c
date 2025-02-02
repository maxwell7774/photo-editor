#include "image.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jpeg_t *open_image(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    return NULL;
  }

  fseek(fp, 0L, SEEK_END);
  uint64_t size = ftell(fp);
  uint8_t *data = malloc(size * sizeof(uint8_t));
  if (data == NULL) {
    fclose(fp);
    return NULL;
  }
  rewind(fp);
  size_t bytes_read = fread(data, 1, size, fp);
  fclose(fp);

  jpeg_t *jpeg_ptr = malloc(sizeof(jpeg_t));
  if (jpeg_ptr == NULL) {
    return NULL;
  }

  jpeg_ptr->size = size;
  jpeg_ptr->data = data;

  printf("BYTES READ: %zu\tSIZE:%zu\n", bytes_read, jpeg_ptr->size);

  return jpeg_ptr;
}

bool save_image(jpeg_t *image_ptr, char *filename) {
    char *sanitized_filename = calloc(strlen(filename) + 1, sizeof(char));
    if(sanitized_filename == NULL) {
        return false;
    }
    strcpy(sanitized_filename, filename);

    for(int i = 0; i < strlen(sanitized_filename); i++) {
        if(sanitized_filename[i] == '.') {
            sanitized_filename[i] = '_';
        }
    }

    size_t path_size = strlen("/res.jpeg") + strlen(sanitized_filename);

    char *path = calloc(path_size+1, sizeof(char));
    if(path == NULL) {
        free(sanitized_filename);
        return false;
    }

    strcat(path, "res/");
    strcat(path, sanitized_filename);
    strcat(path, ".jpeg");
    printf("SAVING IMAGE TO: %s\n", path);
    free(sanitized_filename);

    FILE *fp = fopen(path, "w");
    if(fp == NULL) {
        return false;
    }
    fwrite(image_ptr->data, sizeof(uint8_t), image_ptr->size, fp);
    fclose(fp);

    free(path);
    return true;
}

void delete_image(jpeg_t *image_ptr) {
    free(image_ptr->data);
    free(image_ptr);
}
