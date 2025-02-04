#pragma once
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

enum JPEGMarkerKind {
    NOT_MARKER = 0,
    START_OF_IMAGE,
    START_OF_FRAME,
    HUFFMAN_TABLE,
    QUANTIZATION_TABLE,
    RESTART_INTERVAL,
    START_OF_SCAN,
    RESTART,
    APPLICATION_SPECIFIC,
    COMMENT,
    END_OF_IMAGE,
} typedef jpeg_marker_kind_t ;

struct JPEGMarker {
    jpeg_marker_kind_t kind;
    uint64_t start_pos;
    size_t size;
} typedef jpeg_marker_t;

struct JPEG {
    uint8_t *data;
    jpeg_marker_t **markers;
    int markers_count;
    size_t size;
} typedef jpeg_t;

jpeg_t *open_jpeg(char *filename);
bool save_jpeg(jpeg_t *jpeg, char *filename);
void free_jpeg(jpeg_t *jpeg);
jpeg_marker_kind_t get_marker_kind(uint8_t post_byte);
size_t get_marker_size(jpeg_t *jpeg, int marker_index);
