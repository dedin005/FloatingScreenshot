#include "helperFunctions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

/*
 * width: int => width of image
 * height: int => height of image
 * data: color_t* => array of pixel values
 * filename: char* => null terminated string that defines the output filename; should end with the extension .ppm
 */
int outputImage(int width, int height, color_t *data, char *filename)
{
    const uint8_t COLOR_MAX = 255;

    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        return 1;
    }

    /* write header to the file */
    fprintf(fp, "P6\n%d %d\n%d\n", width, height, COLOR_MAX);

    /* write image data bytes to the file */
    fwrite(data, sizeof(data) * width * height, 1, fp);
    fclose(fp);

    return 0;
}

/*
 * img: XImage* => XImage struct
 * filename: char* => name of output file, should have .ppm extension
 */
int imageToPPM(XImage *img, char *filename)
{
    if (!img)
    {
        return 1;
    }

    uint64_t redMask = img->red_mask;
    uint64_t greenMask = img->green_mask;
    uint64_t blueMask = img->blue_mask;

    color_t data[img->height][img->width];

    for (int j = 0; j < img->width; j++)
    {
        for (int i = 0; i < img->height; i++)
        {
            uint8_t colors[3];
            uint64_t p = XGetPixel(img, j, i);
            colors[0] = (p & redMask) >> 16;
            colors[1] = (p & greenMask) >> 8;
            colors[2] = p & blueMask;

            memcpy((void *)&data[i][j], colors, sizeof(color_t));
        }
    }

    if (outputImage(img->width, img->height, (color_t *)data, filename))
    {
        return 1;
    }

    return 0;
}

int dimImage(XImage *img)
{
    if (!img)
    {
        return 1;
    }

    uint64_t red_mask = img->red_mask;
    uint64_t blue_mask = img->blue_mask;
    uint64_t green_mask = img->green_mask;

    for (int i = 0; i < img->width; i++)
    {
        for (int j = 0; j < img->height; j++)
        {
            uint64_t p = XGetPixel(img, i, j);
            uint8_t red = (p & red_mask) >> 16;
            uint8_t green = (p & green_mask) >> 8;
            uint8_t blue = (p & blue_mask);
            red = red >> 1;
            green = green >> 1;
            blue = blue >> 1;
            XPutPixel(img, i, j, (((uint64_t)red) << 16) | (((uint64_t)green) << 8) | ((uint64_t)blue));
        }
    }
    return 0;
}
