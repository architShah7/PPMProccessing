#include "filter.h"
#include <stdio.h>
#include <stdlib.h>

/* readPPM reads a PPM image from a file.
   Input: file is a pointer to a character string specifying a file name.
   Output:
     *width stores the width of the image
     *height stores its height
     *max stores the largest RGB value in the image

   readPPM returns a pointer to an array of size (*width)*(*height)
   storing the RGB values of the image.
*/
RGB *readPPM(const char *file, int *width, int *height, int *max)
{

    //p3
    //width height
    //max

    FILE *fp;
    RGB *rgb;
    fp = fopen(file, "r");

    fgetc(fp);
    fgetc(fp);
    fscanf(fp, "%d %d", width, height);
    fscanf(fp, "%d", max);

    rgb = (RGB *)malloc(sizeof(RGB) * (*(height) * (*width)));

    if (fp != NULL)
    {

        for (int i = 0; i < ((*height) * (*width)); i++)
        {
            fscanf(fp, "%hhu%hhu%hhu", &rgb[i].r, &rgb[i].g, &rgb[i].b);
        }

        fclose(fp);
    }
    else
    {
        printf("error");
    }
    return rgb;
}

/* writePPM writes an image into a file.
   Input:
      file   name of the file
      width  width of the image
      height height of the image
      max    largest RGB value
      image  pointer to an array of size/ width*height with RGB values
*/
void writePPM(const char *file, int width, int height, int max, const RGB *image)
{

    FILE *fp;
    RGB *rgb;
    fp = fopen(file, "w");

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "%d\n", max);

    for (int i = 0; i < ((height) * (width)); i++)
    {
        fprintf(fp, "%d %d %d\n", image[i].r, image[i].g, image[i].b);
    }

    fclose(fp);
}

/* denoiseImage applies filtering to an image.
   Input:
      width  width of the image
      height height of the image
      max    largest RGB value
      image  pointer to an array of size width*height with RGB values
      n      size of filtering window
      f      type of filtering: MEAN or MEDIAN

   denoiseImage returns a pointer to an arrary of size width*height containing the new image.
*/

RGB *denoiseImage(int width, int height, const RGB *image, int n, filter f)
{

    int row, col;
    RGB *newImg = (RGB *)malloc(sizeof(RGB) * (*(height) * (*width)));

    for (row = 0; row < height; row++)
    {
        for (col = 0; col < width; col++)
        {
            unsigned char *redVals = (unsigned char *)malloc(sizeof(unsigned char) * n * n);
            unsigned char *greenVals = (unsigned char *)malloc(sizeof(unsigned char) * n * n);
            unsigned char *blueVals = (unsigned char *)malloc(sizeof(unsigned char) * n * n);

            int i = 0;
            int left = col - n / 2;
            int right = col + n / 2;
            int up = row - n / 2;
            int down = row + n / 2;
            int k, l;

            for (k = left; k < right; k++)
            {
                for (l = up; l < down; l++)
                {
                    if (k >= 0 && l >= 0 && k < height && l < width)
                    {
                        redVals[i] = image[k * width + l].r;
                        greenVals[i] = image[k * width + l].g;
                        blueVals[i] = image[k * width + l].b;
                        i++;
                    }
                }
            }

            unsigned char newR;
            unsigned char newG;
            unsigned char newB;

            if (f == MEAN)
            {

                int sum = 0, sum1 = 0, sum2 = 0;
                int size = i;
                for (int k = 0; k < i; k++)
                {
                    sum += redVals[k];
                    sum1 += greenVals[k];
                    sum2 += blueVals[k];
                }

                unsigned char mean = (unsigned char)sum /= size;
                unsigned char mean1 = (unsigned char)sum1 /= size;
                unsigned char mean2 = (unsigned char)sum2 /= size;
            }

            if (f == MEDIAN)
            {
                qsort(redVals, size, sizeof(unsigned char), cmp);
                qsort(greenVals, size, sizeof(unsigned char), cmp);
                qsort(blueVals, size, sizeof(unsigned char), cmp);

                newR = redVals[size / 2];
                newG = greenVals[size / 2];
                newB = blueVals[size / 2];
            }

            newImg[row * width + col].r = newR;
            newImg[row * width + col].g = newG;
            newImg[row * width + col].b = newB;
        }
    }

    return newImg;
}
