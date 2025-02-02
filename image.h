#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct JPEG {
    uint8_t *data;
    size_t size;
} typedef jpeg_t;

jpeg_t *open_image(char *filename);
bool save_image(jpeg_t *image_ptr, char *filename);
void delete_image(jpeg_t *image_ptr);
