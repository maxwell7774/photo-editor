#!/bin/bash

clang main.c -o bin/photo-editor

rm res/image_out.jpeg
bin/photo-editor
