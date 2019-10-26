#include "filter.h"
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//find top left, right, bottom, and loop throguh top to bottom, and left to right

int cmpr(const void *a, const void *b);
int cmpg(const void *a, const void *b);
int cmpb(const void *a, const void *b);

RGB *readPPM(const char *file, int *width, int *height, int *max)
{
    int fd;
    size_t bytesRead;
    off_t currLoc;

    fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        printf("Error! Read Failed!");
    }

    char *p = malloc(2 * sizeof(char));
    char *w = malloc(1 * sizeof(int));
    char *t = malloc(40 * sizeof(char));
    unsigned char *m = malloc(1 * sizeof(unsigned char));

    bytesRead = read(fd, p, 2);
    // printf("FL is %s \n", p);
    currLoc = lseek(fd, 3, SEEK_SET);

    bytesRead = read(fd, t, 40);
    //printf("READ IS %s \n", t);

    char del[] = " ";
    char *ptrr = strtok(t, del);
    *width = atoi(ptrr);
    // printf("WIDTH is %d \n", *width);

    while (ptrr != NULL)
    {
        ptrr = strtok(NULL, del);
        //printf("PTRR IS %s", ptrr);
        *height = atoi(ptrr);
        //printf("Size is %ld", sizeof(*height));
        break;
    }

    //printf("Height is %d \n", *height);

    int bytes = floor(log10(abs(*height))) + 1;
    //printf("BYTES IS: %d", bytes);
    int bytesTwo = floor(log10(abs(*width))) + 1;
    // printf("BYTES IS: %d", bytesTwo);
    currLoc = lseek(fd, 5 + bytes + bytesTwo, SEEK_SET);

    bytesRead = read(fd, w, 4);
    *max = atoi(w);
    //printf("Max is %d \n", *max);

    currLoc = lseek(fd, 9 + bytes + bytesTwo, SEEK_SET);

    //printf("Max is %d \n", *max);

    /*
  bytesRead = read(fd, w, 4);
  *width = atoi(w);
  printf("WIDTH is %d \n", *width);
  
  currLoc = lseek(fd, 8, SEEK_SET);
  bytesRead = read(fd, w, 4);
  printf("Height is %s \n", w);
  
  *height = atoi(w);
  currLoc = lseek(fd, 13, SEEK_SET);
  bytesRead = read(fd, w, 4);
  *max = atoi(w);
  printf("Max is %d \n", *max);
  currLoc = lseek(fd, 17, SEEK_SET);
*/
    unsigned char *a = malloc((sizeof(unsigned char) * (*height) * (*width) * 3 * (*width) * 3));

    RGB *rgb;
    rgb = (RGB *)malloc(sizeof(RGB) * ((*height) * (*width)));

    //get file size in bytes
    struct stat sb;

    if (fstat(fd, &sb) == -1)
    {
        perror("Failure");
    }

    off_t filesize = sb.st_size;

    off_t chunk = 0;

    char delim[] = " ";
    currLoc = 0;
    //bytesRead = read(fd, a, filesize);
    //read the entire file
    for (int i = 0; i < ((*height) * (*width)); i++)
    {
        bytesRead = read(fd, a + chunk, 1048576);

        currLoc = lseek(fd, 0, SEEK_CUR);
        //currLoc >= filesize
        if (bytesRead == 0)
        {
            printf("Reached EOF \n");
            break;
        }
        chunk = chunk + bytesRead;
    }
    //printf("READ IS: %s", a);
    char *ptr = strtok(a, delim);
    int pq = atoi(ptr);
    //printf("pixel is: %d \n", pq);
    int totalEntries = (*height) * (*width * 3);
    //printf("TOTAL IS %d \n", total);

    int c = 1;
    int j = 0;
    /*
  while (ptr != NULL)
	{
		  int p = atoi(ptr);
      if(c == 1){
        rgb[j].r = (unsigned char) p;
        printf("R VAL: %d  ", rgb[j].r);
        c++;
      } 
      else if(c == 2){
        rgb[j].g = (unsigned char) p;
        printf("G VAL: %d  ", rgb[j].g);
        c++;

      }else if (c == 3){
          rgb[j].b = (unsigned char) p;
          printf("B VAL: %d  \n", rgb[j].b);
          c = 1;
          j++;
          
      }
      ptr = strtok(NULL, delim);
      

     
    
  }*/

    while (ptr != NULL)
    {
        //printf("Sec 5");
        if (j >= (*height) * (*width))
        {
            break;
        }
        int p = atoi(ptr);
        rgb[j].r = (unsigned char)p;
        // printf("R VAL: %d  ", rgb[j].r);

        ptr = strtok(NULL, delim);
        p = atoi(ptr);
        rgb[j].g = (unsigned char)p;
        //printf("G VAL: %d  ", rgb[j].g);

        ptr = strtok(NULL, delim);
        p = atoi(ptr);
        rgb[j].b = (unsigned char)p;
        //printf("B VAL: %d  \n", rgb[j].b);

        //printf("Sec 1 \n");

        ptr = strtok(NULL, delim);
        if (ptr == NULL || j + 1 >= (*height) * (*width))
        {
            break;
        }
        p = atoi(ptr);
        rgb[j + 1].r = (unsigned char)p;
        //printf("R VAL: %d  ", rgb[j+1].r);

        ptr = strtok(NULL, delim);
        p = atoi(ptr);
        rgb[j + 1].g = (unsigned char)p;
        //printf("G VAL: %d  ", rgb[j+1].g);

        ptr = strtok(NULL, delim);
        p = atoi(ptr);
        rgb[j + 1].b = (unsigned char)p;
        //printf("B VAL: %d  \n", rgb[j+1].b);

        //printf("Sec 2 \n");

        ptr = strtok(NULL, delim);
        if (ptr == NULL || j + 2 >= (*height) * (*width))
        {
            break;
        }
        p = atoi(ptr);
        rgb[j + 2].r = (unsigned char)p;
        //printf("R VAL: %d  ", rgb[j+2].r);

        ptr = strtok(NULL, delim);
        p = atoi(ptr);
        rgb[j + 2].g = (unsigned char)p;
        //printf("G VAL: %d  ", rgb[j+2].g);

        ptr = strtok(NULL, delim);
        p = atoi(ptr);
        rgb[j + 2].b = (unsigned char)p;
        //printf("B VAL: %d  \n", rgb[j+2].b);

        //printf("Sec 3 \n");

        ptr = strtok(NULL, delim);
        if (ptr == NULL || j + 3 >= (*height) * (*width))
        {
            break;
        }
        p = atoi(ptr);
        rgb[j + 3].r = (unsigned char)p;
        //printf("R VAL: %d  ", rgb[j+3].r);

        ptr = strtok(NULL, delim);
        p = atoi(ptr);
        rgb[j + 3].g = (unsigned char)p;
        //printf("G VAL: %d  ", rgb[j+3].g);

        ptr = strtok(NULL, delim);
        p = atoi(ptr);
        rgb[j + 3].b = (unsigned char)p;
        //printf("B VAL: %d  \n", rgb[j+3].b);

        ptr = strtok(NULL, delim);
        j += 4;
    }

    /*
  for(i = 0; i < ((*height) * (*width)); i++){
    bytesRead = read(fd, a, 120);
    char *ptr = strtok(a, delim);
    int count = 0;

    while (ptr != NULL)
	  {
		  int r = atoi(ptr);
      //printf("pixel is: %d \n", r);
      ptr = strtok(NULL, delim);
      
	  }
   
    

    currLoc = lseek(fd, 0, SEEK_CUR);
    //printf("Curr locaiton is: %d \n", currLoc);
    if(currLoc >= filesize){
      printf("Reached EOF \n");
      break;
    }

    //rgb+chunk

  }
*/
    /*for(int i = 0; i < ((*height) * (*width)); i++){
    printf("%d", rgb[i].r);
  }*/

    close(fd);
    return rgb;
}

void writePPM(const char *file, int width, int height, int max, const RGB *image)
{

    FILE *fp;
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

int newCount(int n, const RGB *image, int *sumR, int *sumG, int *sumB, int height, int width, int row, int col)
{
    int count = 0, k = 0, l = 0;
    int sR = 0, sG = 0, sB = 0;
    for (k = row - n / 2; k <= row + n / 2; k++)
    {
        //printf("k is %d \n", k);

        for (l = 0; l <= n / 2; l++)
        {
            //printf("l is %d \n", l);
            if (k >= 0 && l >= 0 && k < height && l < width)
            {
                count++;
                sR += image[k * width + l].r;
                sG += image[k * width + l].g;
                sB += image[k * width + l].b;
            }
        }
    }

    *sumR = sR;
    *sumG = sG;
    *sumB = sB;

    return count;
}

int newCt(int n, int *windowR, int *windowG, int *windowB, RGB *removeCol, const RGB *image, int width, int height, int row)
{
    int count = 0, k = 0, l = 0, i = 0, p = 0;

    for (k = row - n / 2; k <= row + n / 2; k++)
    {
        for (l = 0; l <= n / 2; l++)
        {
            if (k >= 0 && l >= 0 && k < height && l < width)
            {

                count++;

                windowR[i] = image[k * width + l].r;
                windowG[i] = image[k * width + l].g;
                windowB[i] = image[k * width + l].b;

                if (l == 0)
                {
                    removeCol[p].r = image[k * width + l].r;
                    removeCol[p].g = image[k * width + l].g;
                    removeCol[p].b = image[k * width + l].b;
                    p++;
                }

                i++;
            }
        }
    }

    return count;
}

void printCol(int *removeCol, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("i: %d Val: %d   ", i, removeCol[i]);
    }

    printf(" \n");
}

int cmp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int binary_search(int *sorted_list, int low, int high, int element)
{
    int middle;
    while (low <= high)
    {
        middle = low + (high - low) / 2;
        if (element > sorted_list[middle])
            low = middle + 1;
        else if (element < sorted_list[middle])
            high = middle - 1;
        else
            return middle;
    }
    return -1;
}

RGB *denoiseImage(int width, int height, const RGB *image, int n, filter f)
{
    RGB *newImg = NULL, *window = NULL, *removeCol = NULL;
    int *windowR = NULL, *windowG = NULL, *windowB = NULL;
    int *vals = NULL;

    if (f == MEAN)
    {
        int row, col;

        newImg = (RGB *)malloc(sizeof(RGB) * ((height) * (width)));
        window = (RGB *)malloc(sizeof(RGB) * n * n);

        vals = (int *)malloc(sizeof(int) * (n / 2) + 1);

        int sumR = 0, sumG = 0, sumB = 0, iter = 0, rev = n / 2 - 1, t = 1;

        int k = 0, l = 0;

        /*going through each pixel, extracting values for r,g,b  */

        int count = 0, p = 0;

        for (k = 0; k <= n / 2; k++)
        {
            //printf("k is %d \n", k);

            for (l = 0; l <= n / 2; l++)
            {
                //printf("l is %d \n", l);
                if (k >= 0 && l >= 0 && k < height && l < width)
                {
                    count++;
                    //printf("Count: %d,   index is %d \n", count, image[k * width + l].r);

                    sumR += image[k * width + l].r;
                    sumG += image[k * width + l].g;
                    sumB += image[k * width + l].b;
                }
            }
        }

        vals[p] = count;
        p++;
        iter++;
        newImg[0].r = (unsigned char)(sumR / count);
        newImg[0].g = (unsigned char)(sumG / count);
        newImg[0].b = (unsigned char)(sumB / count);

        for (row = 0; row < height; row++)
        {

            if (row == 0)
            {
                col = 1;
            }
            else
            {
                col = 0;
                iter++;
            }

            for (; col < width; col++)
            {
                //printf("-------------- Row is %d Col is %d ------------------- \n", row, col);
                int x = 0;

                l = col + (n / 2);

                /*Adding next col */
                for (k = row - n / 2; k <= row + (n / 2); k++)
                {
                    //printf("ADD Col is %d \n", l);
                    if (k >= 0 && l >= 0 && k < height && l < width)
                    {
                        //x++;
                        count++;
                        sumR += image[k * width + l].r;
                        sumG += image[k * width + l].g;
                        sumB += image[k * width + l].b;
                    }
                }

                //printf("Count1: %d \n", x);
                int y = 0;
                l = col - (n / 2) - 1;

                for (k = row - (n / 2); k <= row + (n / 2); k++)
                {

                    if (k >= 0 && l >= 0 && k < height && l < width)
                    {
                        //printf("K val is %d \n", k);
                        y++;
                        count--;
                        //printf("Count2: %d \n");
                        sumR -= image[k * width + l].r;
                        sumG -= image[k * width + l].g;
                        sumB -= image[k * width + l].b;
                    }
                }

                if (col == 0)
                {
                    count = newCount(n, image, &sumR, &sumG, &sumB, height, width, row, col);
                }
                //printf("SIZE is %d \n", count);

                //printf("SUM R: %d \n", (unsigned char)(sumR/count));
                newImg[row * width + col].r = (unsigned char)(sumR / count);
                newImg[row * width + col].g = (unsigned char)(sumG / count);
                newImg[row * width + col].b = (unsigned char)(sumB / count);
            }
        }
    }

    //Count stuff is a problem now, vals to be removed are printing fine
    else if (f == MEDIAN)
    {
        int row = 0, col = 0, remIndex = 0, remIndexR = 0, remIndexG = 0, remIndexB = 0;

        newImg = (RGB *)malloc(sizeof(RGB) * ((height) * (width)));
        windowR = (int *)malloc(sizeof(int) * n * n);
        windowG = (int *)malloc(sizeof(int) * n * n);
        windowB = (int *)malloc(sizeof(int) * n * n);

        removeCol = (RGB *)malloc(sizeof(RGB) * height);

        int sumR = 0, sumG = 0, sumB = 0;

        int k = 0, l = 0;

        /*Sort the initial array */

        int count = 0, p = 0, i = 0;

        /*Load the first n values in the window object */
        for (k = 0; k <= n / 2; k++)
        {
            for (l = 0; l <= n / 2; l++)
            {

                if (k >= 0 && l >= 0 && k < height && l < width)
                {

                    count++;
                    //printf("Count: %d,   index is %d \n", count, image[k * width + l].r);
                    windowR[i] = image[k * width + l].r;
                    windowG[i] = image[k * width + l].g;
                    windowB[i] = image[k * width + l].b;

                    if (l == 0)
                    {
                        removeCol[p].r = image[k * width + l].r;
                        removeCol[p].g = image[k * width + l].g;
                        removeCol[p].b = image[k * width + l].b;
                        p++;
                    }

                    i++;
                }
            }
        }

        printf("Row is %d, col is %d Image indexes to be removed: ", row, col);
        //printCol(removeCol, height);

        /*Sort it */
        for (int u = 0; u < count; u++)
        {
            printf("%d   ", windowR[u]);
        }
        printf(" \n");

        qsort(windowR, count, sizeof(int), cmp);
        newImg[row * width + col].r = (unsigned char)(windowR[count / 2]);

        qsort(windowG, count, sizeof(int), cmp);
        newImg[row * width + col].g = (unsigned char)(windowG[count / 2]);

        qsort(windowB, count, sizeof(int), cmp);
        newImg[row * width + col].b = (unsigned char)(windowB[count / 2]);

        int numRemoved = p;
        int netCount = count;

        for (row = 0; row < height; row++)
        {
            col = (row == 0 ? 1 : 0);

            // printf("---------------------------------------- \n");

            for (; col < width; col++)
            {

                int x = 0;

                l = col + (n / 2);
                printf("Row is %d, col is %d  --------------- \n ", row, col);
                for (k = row - n / 2; k <= row + (n / 2); k++)
                {

                    if (k >= 0 && l >= 0 && k < height && l < width && col != 0)
                    {
                        if (x <= numRemoved)
                        {
                            printf("A \n");
                            //printf("IF \n");
                            for (int u = 0; u < count; u++)
                            {
                                printf("%d   ", windowR[u]);
                            }

                            printf(" \n");
                            printf("Min is %d \n", windowR[0]);
                            printf("Max %d \n", windowR[count - 1]);
                            printf("Val to be removed is %d \n", removeCol[x].r);
                            remIndexR = binary_search(windowR, 0, count - 1, (int)(removeCol[x].r));
                            printf("Index to be removed is %d \n", remIndexR);
                            remIndexG = binary_search(windowG, windowG[0], windowG[count - 1], removeCol[x].g);
                            remIndexB = binary_search(windowB, windowB[0], windowB[count - 1], removeCol[x].b);
                        }
                        else
                        {
                            //printf("ELSE \n");
                            remIndexR = numRemoved;
                            remIndexG = numRemoved;
                            remIndexB = numRemoved;
                        }

                        printf("B \n");

                        //clcremIndex = removeCol[x];
                        //  printf("RemIndex is %d, add Index is %d \n", remIndex, k * width + l);
                        x++;
                        //count++;
                        netCount++;
                        windowR[remIndexR] = image[k * width + l].r;
                        windowG[remIndexG] = image[k * width + l].g;
                        windowB[remIndexB] = image[k * width + l].b;

                        qsort(windowR, netCount, sizeof(int), cmp);
                        qsort(windowG, netCount, sizeof(int), cmp);
                        qsort(windowB, netCount, sizeof(int), cmp);
                    }
                    else if (k >= 0 && l >= 0 && k < height && l < width && col == 0)
                    {
                        count = newCt(n, windowR, windowG, windowB, removeCol, image, width, height, row);
                        netCount = count;
                        x = count;
                    }
                }

                int y = 0;
                //        l = col - (n / 2);
                l = col + (n / 2);

                //l = col - (n / 2) - 1;
                for (k = row - (n / 2); k <= row + (n / 2); k++)
                {
                    printf("IN Remove");
                    if (k >= 0 && l >= 0 && k < height && l < width)
                    {
                        netCount--;
                        printf("HERE %d \n", image[k * width + (l - 1)].r);
                        removeCol[y].r = image[k * width + l].r;
                        removeCol[y].g = image[k * width + l].g;
                        removeCol[y].b = image[k * width + l].b;

                        y++;
                        //Get the indexes to be removed
                    }
                }

                //printf("Row is %d, col is %d  ---------------  Values to be removed: ", row, col);
                //printCol(removeCol, height);

                // printf("\n");
                numRemoved = y;

                // printf("Count is %d \n", count);
                qsort(windowR, netCount, sizeof(int), cmp);
                newImg[row * width + col].r = (unsigned char)(windowR[netCount / 2]);
                qsort(windowG, netCount, sizeof(int), cmp);
                newImg[row * width + col].g = (unsigned char)(windowG[netCount / 2]);
                qsort(windowB, netCount, sizeof(int), cmp);
                newImg[row * width + col].b = (unsigned char)(windowB[netCount / 2]);
            }
        }
    }

    return newImg;
}

int cmpr(const void *a, const void *b)
{
    unsigned char x = ((RGB *)a)->r;
    unsigned char y = ((RGB *)b)->r;
    if (x < y)
        return -1;
    if (x > y)
        return 1;
    return 0;
}

int cmpg(const void *a, const void *b)
{
    unsigned char x = ((RGB *)a)->g;
    unsigned char y = ((RGB *)b)->g;
    if (x < y)
        return -1;
    if (x > y)
        return 1;
    return 0;
}

int cmpb(const void *a, const void *b)
{
    unsigned char x = ((RGB *)a)->b;
    unsigned char y = ((RGB *)b)->b;
    if (x < y)
        return -1;
    if (x > y)
        return 1;
    return 0;
}
