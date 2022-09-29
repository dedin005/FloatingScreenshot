#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <stdint.h>
#include <X11/Xlib.h>

typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} __attribute__((packed)) color_t;

typedef struct
{
    uint8_t x;
    uint8_t y;
} pointerPosition;

int imageToPPM(XImage *img, char *filename);

int _XlibErrorHandler(Display *display, XErrorEvent *event);

#endif /* HELPER_FUNCTIONS_H */