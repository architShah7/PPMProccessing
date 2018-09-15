/* file main_bst.c */
#include <stdio.h>
#include <stdlib.h>

#include "filter.h"

extern double getTime();
int main(int argc, char *argv[]) {

  if (argc != 4 )
    {
      printf("\n Usage: ");
      return 1;
    }

  double time = getTime();
  char *inFile = argv[0], *outFile = argv[1];
  int N = atoi(argv[2]);
  char typeFil = argv[3];

  int height, width, max;
  filter f;


  printf("Reading file %c\n",*inFile);
  RBG *values = readPPM(inFile, &width, &height, &max);
  printf("%c read in %.1e seconds\n", time);


  if(typeFil == 'M'){f = MEDIAN;}
  else if (typeFil == 'A'){f = MEAN;}

  printf("Proccessing %d x %d image using %d x %d window and median filter", width, height);
  values = denoiseImage(width,height,values, N, f);
  printf("Image processed in %.1e seconds\n", time);
  printf("\n");


  printf("Writing file %c\n",*inFile);
  writePPM(outFile, width, height, max, values);
  printf("%c written in %.1e seconds\n", time);
  printf("\n");


  free(A);
  return 0;
}
