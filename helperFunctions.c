#include "helperFunctions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <png.h>

/*
 * filename: char* => null terminated string that defines the output filename; should end with the extension .ppm
 * data: uint_t** => array of pointers to image data
 * width: int => width of image
 * height: int => height of image
 */
int outputImagePNG(char *filename, uint8_t **data, int width, int height)
{
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        return (ERROR);
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        return (ERROR);
    }
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        return (ERROR);
    }
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return (ERROR);
    }
    png_init_io(png_ptr, fp);
    int bit_depth = 8,
        color_type = PNG_COLOR_TYPE_RGB,
        interlace_type = PNG_INTERLACE_NONE,
        compression_type = PNG_COMPRESSION_TYPE_DEFAULT,
        filter_method = PNG_FILTER_TYPE_DEFAULT;
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_type, compression_type, filter_method);
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, (uint8_t **)data);
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return 0;
}

/*
 * width: int => width of image
 * height: int => height of image
 * data: color_t* => array of pixel values
 * filename: char* => null terminated string that defines the output filename; should end with the extension .ppm
 */
int outputImagePPM(int width, int height, color_t *data, char *filename)
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

    if (outputImagePPM(img->width, img->height, (color_t *)data, filename))
    {
        return 1;
    }

    return 0;
}

/*
 * img: XImage* => XImage struct
 * filename: char* => name of output file, should have .png extension
 */
int imageToPNG(XImage *img, char *filename)
{
    if (!img || !filename)
    {
        return 1;
    }

    uint64_t redMask = img->red_mask;
    uint64_t greenMask = img->green_mask;
    uint64_t blueMask = img->blue_mask;

    color_t *data[img->height];

    for (int i = 0; i < img->height; i++)
    {
        color_t *row = malloc(img->width * sizeof(color_t));
        for (int j = 0; j < img->width; j++)
        {
            uint8_t colors[3];
            uint64_t p = XGetPixel(img, j, i);
            colors[0] = (p & redMask) >> 16;
            colors[1] = (p & greenMask) >> 8;
            colors[2] = p & blueMask;

            memcpy((void *)&row[j], colors, sizeof(color_t));
        }
        data[i] = row;
    }

    if (outputImagePNG(filename, (uint8_t **)data, img->width, img->height))
    {
        return 1;
    }

    for (int i = 0; i < img->height; i++)
    {
        free(data[i]);
    }

    return 0;
}

// used to dim the pixel values for the overlay
// doesn't actually work yet
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
