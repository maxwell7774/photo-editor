#include "jpeg.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jpeg_t *open_jpeg(char *filename) {
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

  jpeg_t *jpeg = malloc(sizeof(jpeg_t));
  if (jpeg == NULL) {
    return NULL;
  }

  jpeg->size = size;
  jpeg->data = data;
  jpeg->markers_count = 0;
  jpeg->markers = NULL;

  printf("BYTES READ: %zu\tSIZE:%zu\n", bytes_read, jpeg->size);

  for (int i = 0; i < jpeg->size; i++) {
    uint8_t byte = jpeg->data[i];
    if (byte == 0xFF && i < jpeg->size - 1) {
      jpeg_marker_kind_t marker_kind = get_marker_kind(jpeg->data[i + 1]);
      if (marker_kind != NOT_MARKER) {
        jpeg->markers_count++;
      }
    }
  }

  jpeg->markers = calloc(jpeg->markers_count, sizeof(jpeg_marker_t *));
  if (jpeg->markers == NULL) {
    free_jpeg(jpeg);
    return NULL;
  }

  int marker_index = 0;
  for (int i = 0; i < jpeg->size; i++) {
    uint8_t byte = jpeg->data[i];
    if (byte == 0xFF && i < jpeg->size - 1) {
      jpeg_marker_kind_t marker_kind = get_marker_kind(jpeg->data[i + 1]);
      if (marker_kind != NOT_MARKER) {
        jpeg_marker_t *marker = malloc(sizeof(jpeg_marker_t));
        if (marker == NULL) {
          free_jpeg(jpeg);
          return NULL;
        }
        marker->kind = marker_kind;
        marker->start_pos = i;
        jpeg->markers[marker_index] = marker;
        marker_index++;
      }
    }
  }

  for (int i = 0; i < jpeg->markers_count; i++) {
    jpeg->markers[i]->size = get_marker_size(jpeg, i);
  }

  printf("MARKERS FOUND: %d\n", marker_index);

  return jpeg;
}

bool save_jpeg(jpeg_t *jpeg, char *filename) {
  char *sanitized_filename = calloc(strlen(filename) + 1, sizeof(char));
  if (sanitized_filename == NULL) {
    return false;
  }
  strcpy(sanitized_filename, filename);

  for (int i = 0; i < strlen(sanitized_filename); i++) {
    if (sanitized_filename[i] == '.') {
      sanitized_filename[i] = '_';
    }
  }

  size_t path_size = strlen("/res.jpeg") + strlen(sanitized_filename);

  char *path = calloc(path_size + 1, sizeof(char));
  if (path == NULL) {
    free(sanitized_filename);
    return false;
  }

  strcat(path, "res/");
  strcat(path, sanitized_filename);
  strcat(path, ".jpeg");
  printf("SAVING IMAGE TO: %s\n", path);
  free(sanitized_filename);

  FILE *fp = fopen(path, "w");
  if (fp == NULL) {
    free(path);
    return false;
  }
  fwrite(jpeg->data, sizeof(uint8_t), jpeg->size, fp);
  fclose(fp);
  printf("SAVED IMAGE SUCCESSFULLY\n");

  free(path);
  return true;
}

void free_jpeg(jpeg_t *jpeg) {
  free(jpeg->data);
  for (int i = 0; i < jpeg->markers_count; i++) {
    free(jpeg->markers[i]);
  }
  free(jpeg->markers);
  free(jpeg);
}

jpeg_marker_kind_t get_marker_kind(uint8_t post_byte) {
  switch (post_byte) {
  case 0xD8:
    return START_OF_IMAGE;
  case 0xC0:
  case 0xC2:
    return START_OF_FRAME;
  case 0xC4:
    return HUFFMAN_TABLE;
  case 0xDB:
    return QUANTIZATION_TABLE;
  case 0xDD:
    return RESTART_INTERVAL;
  case 0xDA:
    return START_OF_SCAN;
  case 0xD0:
  case 0xD1:
  case 0xD2:
  case 0xD3:
  case 0xD4:
  case 0xD5:
  case 0xD6:
  case 0xD7:
    return RESTART;
  case 0xE0:
  case 0xE1:
  case 0xE2:
  case 0xE3:
  case 0xE4:
  case 0xE5:
  case 0xE6:
  case 0xE7:
  case 0xE8:
  case 0xE9:
  case 0xEA:
  case 0xEB:
  case 0xEC:
  case 0xED:
  case 0xEE:
  case 0xEF:
    return APPLICATION_SPECIFIC;
  case 0xFE:
    return COMMENT;
  case 0xD9:
    return END_OF_IMAGE;
  default:
    return NOT_MARKER;
  }
}

size_t get_marker_size(jpeg_t *jpeg, int marker_index) {
  if (marker_index == jpeg->markers_count - 1) {
    return jpeg->size - jpeg->markers[marker_index]->start_pos;
  }
  return jpeg->markers[marker_index + 1]->start_pos -
         jpeg->markers[marker_index]->start_pos;
}
