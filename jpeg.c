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
  jpeg_marker_list_t *marker_list = new_jpeg_marker_list(jpeg);
  if (marker_list == NULL) {
    free_jpeg(jpeg);
    return NULL;
  }
  jpeg->marker_list = marker_list;

  printf("BYTES READ: %zu\tSIZE:%zu\n", bytes_read, jpeg->size);

  return jpeg;
}

int get_jpeg_markers_count(jpeg_t *jpeg) {
  int num_of_markers = 0;

  for (uint64_t i = 0; i < jpeg->size; i++) {
    uint8_t byte = jpeg->data[i];
    if (byte == 0xFF && i + 1 < jpeg->size) {
      uint8_t next_byte = jpeg->data[i + 1];
      switch (next_byte) {
      case 0xD8:
      case 0xC0:
      case 0xC4:
      case 0xDB:
      case 0xDD:
      case 0xDA:
      case 0xD0:
      case 0xD1:
      case 0xD2:
      case 0xD3:
      case 0xD4:
      case 0xD5:
      case 0xD6:
      case 0xD7:
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
      case 0xFE:
      case 0xD9:
        num_of_markers++;
      }
    }
  }
  return num_of_markers;
}

jpeg_marker_list_t *new_jpeg_marker_list(jpeg_t *jpeg) {
  int marker_count = get_jpeg_markers_count(jpeg);
  jpeg_marker_list_t *marker_list = malloc(sizeof(jpeg_marker_list_t));
  if (marker_list == NULL) {
    return NULL;
  }
  marker_list->size = marker_count;
  marker_list->count = 0;

  jpeg_marker_t **elements =
      calloc(marker_list->count, sizeof(jpeg_marker_t *));
  if (elements == NULL) {
    free_jpeg_marker_list(marker_list);
    return NULL;
  }
  marker_list->elements = elements;

  uint64_t i = 0;
  for (; i < jpeg->size; i++) {
    uint8_t byte = jpeg->data[i];

    if (byte == 0xFF && i + 1 < jpeg->size) {
      uint8_t next_byte = jpeg->data[i + 1];
      switch (next_byte) {
      case 0xD8: {
        jpeg_marker_t *marker = new_jpeg_marker(START_OF_IMAGE, i, 0);
        if (marker == NULL) {
          free_jpeg_marker_list(marker_list);
          return NULL;
        }
        marker_list_append(marker_list, marker);
        printf("start of image!\n");
        break;
      }
      case 0xC0: {
        set_last_marker_size(marker_list, i);
        jpeg_marker_t *marker = new_jpeg_marker(START_OF_FRAME, i, 0);
        if (marker == NULL) {
          free_jpeg_marker_list(marker_list);
          return NULL;
        }
        marker_list_append(marker_list, marker);
        printf("start of frame\n");
        break;
      }
      case 0xC4: {
        set_last_marker_size(marker_list, i);
        jpeg_marker_t *marker = new_jpeg_marker(HUFFMAN_TABLE, i, 0);
        if (marker == NULL) {
          free_jpeg_marker_list(marker_list);
          return NULL;
        }
        marker_list_append(marker_list, marker);
        printf("huffman table\n");
        break;
      }
      case 0xDB: {
        set_last_marker_size(marker_list, i);
        jpeg_marker_t *marker = new_jpeg_marker(QUANTIZATION_TABLE, i, 0);
        if (marker == NULL) {
          free_jpeg_marker_list(marker_list);
          return NULL;
        }
        marker_list_append(marker_list, marker);
        printf("quantization table\n");
        break;
      }
      case 0xDD: {
        set_last_marker_size(marker_list, i);
        jpeg_marker_t *marker = new_jpeg_marker(RESTART_INTERVAL, i, 0);
        if (marker == NULL) {
          free_jpeg_marker_list(marker_list);
          return NULL;
        }
        marker_list_append(marker_list, marker);
        printf("restart interval\n");
        break;
      }
      case 0xDA: {
        set_last_marker_size(marker_list, i);
        jpeg_marker_t *marker = new_jpeg_marker(START_OF_SCAN, i, 0);
        if (marker == NULL) {
          free_jpeg_marker_list(marker_list);
          return NULL;
        }
        marker_list_append(marker_list, marker);
        printf("start of scan\n");
        break;
      }
      case 0xD0:
      case 0xD1:
      case 0xD2:
      case 0xD3:
      case 0xD4:
      case 0xD5:
      case 0xD6:
      case 0xD7: {
        set_last_marker_size(marker_list, i);
        jpeg_marker_t *marker = new_jpeg_marker(RESTART, i, 0);
        if (marker == NULL) {
          free_jpeg_marker_list(marker_list);
          return NULL;
        }
        marker_list_append(marker_list, marker);
        printf("restart\n");
        break;
      }
      case 0xE0:
      case 0xE1:
      case 0xE2:
      case 0xE3:
      case 0xE4:
      case 0xE5:
      case 0xE6:
      case 0xE7:
      case 0xE8:
      case 0xE9: {
        set_last_marker_size(marker_list, i);
        jpeg_marker_t *marker = new_jpeg_marker(APPLICATION_SPECIFIC, i, 0);
        if (marker == NULL) {
          free_jpeg_marker_list(marker_list);
          return NULL;
        }
        marker_list_append(marker_list, marker);
        printf("application specific\n");
        break;
      }
      case 0xFE: {
        set_last_marker_size(marker_list, i);
        jpeg_marker_t *marker = new_jpeg_marker(COMMENT, i, 0);
        if (marker == NULL) {
          free_jpeg_marker_list(marker_list);
          return NULL;
        }
        marker_list_append(marker_list, marker);
        printf("comment\n");
      }
      case 0xD9: {
        set_last_marker_size(marker_list, i);
        jpeg_marker_t *marker = new_jpeg_marker(END_OF_IMAGE, i, 0);
        if (marker == NULL) {
          free_jpeg_marker_list(marker_list);
          return NULL;
        }
        marker_list_append(marker_list, marker);
        printf("end of image\n");
        break;
      }
      }
    }
  }
  return marker_list;
}

jpeg_marker_t *new_jpeg_marker(jpeg_marker_kind_t kind, uint64_t start,
                               size_t size) {
  jpeg_marker_t *marker = malloc(sizeof(jpeg_marker_t));
  if (marker == NULL) {
    return NULL;
  }

  marker->kind = kind;
  marker->start = start;
  marker->size = size;
  return marker;
}

void marker_list_append(jpeg_marker_list_t *marker_list,
                        jpeg_marker_t *marker) {
  marker_list->elements[marker_list->count] = marker;
  marker_list->count++;
}

void set_last_marker_size(jpeg_marker_list_t *marker_list, uint64_t offset) {
  marker_list->elements[marker_list->count - 1]->size =
      offset - marker_list->elements[marker_list->count - 1]->start;
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
  if (jpeg->marker_list != NULL) {
    free_jpeg_marker_list(jpeg->marker_list);
  }
  free(jpeg->data);
  free(jpeg);
}

void free_jpeg_marker_list(jpeg_marker_list_t *marker_list) {
  printf("COUNT OF MARKERS: %d\n", marker_list->count);
  for (int i = 0; i < marker_list->count; i++) {
    printf("FREEING MARKER: %d\n", i);
    jpeg_marker_t *marker = marker_list->elements[i];
    printf("Marker START: %zu\n", marker->start);
    printf("Marker SIZE: %zu\n", marker->size);
    printf("Marker KIND: %d\n", marker->kind);
    free(marker_list->elements[i]);
  }
  printf("FREEING ELEMENTS\n");
  free(marker_list->elements);
  printf("FREEING MARKER LIST\n");
  free(marker_list);
  printf("FREED MARKER LIST\n");
}
