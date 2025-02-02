#pragma once
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

enum JPEGMarkerKind {
    START_OF_IMAGE = 1,
    START_OF_FRAME = 2,
    HUFFMAN_TABLE = 3,
    QUANTIZATION_TABLE = 4,
    RESTART_INTERVAL = 5,
    START_OF_SCAN = 6,
    RESTART = 7,
    APPLICATION_SPECIFIC = 8,
    COMMENT = 9,
    END_OF_IMAGE = 10,
} typedef jpeg_marker_kind_t ;

struct JPEGMarker {
    jpeg_marker_kind_t kind;
    uint64_t start;
    size_t size;
} typedef jpeg_marker_t;

struct JPEGMarkerList {
    size_t size;
    int count;
    jpeg_marker_t **elements;
} typedef jpeg_marker_list_t;

struct JPEG {
    uint8_t *data;
    jpeg_marker_list_t *marker_list;
    size_t size;
} typedef jpeg_t;

jpeg_t *open_jpeg(char *filename);
int get_jpeg_markers_count(jpeg_t *jpeg);
jpeg_marker_list_t *new_jpeg_marker_list(jpeg_t *jpeg);
jpeg_marker_t *new_jpeg_marker(jpeg_marker_kind_t, uint64_t start, size_t size);
void marker_list_append(jpeg_marker_list_t *marker_list, jpeg_marker_t *marker);
void set_last_marker_size(jpeg_marker_list_t *marker_list, uint64_t offset);
bool save_jpeg(jpeg_t *jpeg, char *filename);
void free_jpeg(jpeg_t *jpeg);
void free_jpeg_marker_list(jpeg_marker_list_t *marker_list);
