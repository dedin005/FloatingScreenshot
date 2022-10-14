#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <stdint.h>
#include <X11/Xlib.h>

#define ERROR 1

typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} __attribute__((packed)) color_t;

// Takes img and writes to .ppm file
int imageToPPM(XImage *img, char *filename);

// Takes pixel array and writes .png file
int imageToPNG(XImage *img, char *filename);

// Dims image by 50%
int dimImage(XImage *img);

#endif /* HELPER_FUNCTIONS_H */