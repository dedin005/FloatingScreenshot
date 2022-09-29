#include "helperFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#define CLICK 256

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage:\n  %s <outputFilename>.ppm [Photo Viewer]\n", argv[0]);
        return 1;
    }
    char *photoRunner = "feh";
    if (argc > 2)
    {
        photoRunner = argv[2];
    }

    Display *display = XOpenDisplay(None);
    Window window_returned;

    XGrabPointer(
        display,
        DefaultRootWindow(display),
        False,
        ButtonPressMask,
        GrabModeAsync,
        GrabModeAsync,
        DefaultRootWindow(display),
        XCreateFontCursor(display, XC_crosshair),
        CurrentTime);

    int start_x, start_y, stop_x, stop_y, root_x, root_y, win_x, win_y;

    uint8_t pressed = 0;

    uint32_t mask;
    while (1)
    {
        XQueryPointer(
            display, DefaultRootWindow(display),
            &window_returned, &window_returned,
            &root_x, &root_y, &win_x, &win_y,
            &mask);
        if (mask & CLICK && !pressed)
        {
            start_x = root_x;
            start_y = root_y;
            pressed = 1;
        }
        else if (pressed && !(mask & CLICK))
        {
            stop_x = root_x;
            stop_y = root_y;
            break;
        }
    }

    XUngrabPointer(display, CurrentTime);

    uint64_t x = start_x > stop_x ? stop_x : start_x; // y ^ ((x ^ y) & -(x < y)) => min
    uint64_t y = start_y > stop_y ? stop_y : start_y; // y ^ ((x ^ y) & -(x < y)) => min

    uint64_t width = start_x - stop_x < 0 ? stop_x - start_x : start_x - stop_x;
    uint64_t height = start_y - stop_y < 0 ? stop_y - start_y : start_y - stop_y;

    XImage *sc = XGetImage(
        display,
        DefaultRootWindow(display),
        x,
        y,
        width,
        height,
        AllPlanes,
        ZPixmap);

    if (imageToPPM(sc, argv[1]))
    {
        printf("An error occurred while trying to create the screenshot.\n");
        XCloseDisplay(display);
        return 1;
    }

    XCloseDisplay(display);

    pid_t child = fork();
    if (!child)
    {
        // child
        execlp(photoRunner, photoRunner, argv[1], NULL);
    }
    else if (child < 0)
    {
        // error
        printf("An error occurred while trying to open screenshot.\n");
        printf("You can open it at %s.\n", argv[1]);
        return 1;
    }

    return 0;
}
