#include <stdio.h>
#include <X11/Xlib.h>

int main() {}

void getPointer(int *root_x, int *root_y)
{
    int number_of_screens, i, win_x, win_y;
    unsigned int mask_return;
    Bool result;
    Window *root_windows;
    Window window_returned;

    Display *display = XOpenDisplay(NULL);

    number_of_screens = XScreenCount(display);

    root_windows = malloc(sizeof(Window) * number_of_screens);
    for (i = 0; i < number_of_screens; i++)
    {
        root_windows[i] = XRootWindow(display, i);
    }
    for (i = 0; i < number_of_screens; i++)
    {
        result = XQueryPointer(display, root_windows[i],
                               &window_returned, &window_returned, root_x,
                               root_y, &win_x, &win_y, &mask_return);
        if (result)
            break;
    }
    if (!result)
    {
        fprintf(stderr, "No mouse found.\n");
        return;
    }
    free(root_windows);

    XCloseDisplay(display);
}