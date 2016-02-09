#include <stdio.h>
#include <stdlib.h>
#include "util.h"


int main(int argc, char** argv)
{
    int i, j;
    int height = 800;
    int width = 1200;
    double dx = 3./width;
    double dy = 2./height;
    double cr, ci;
    FILE *f;
    int *image = (int *)malloc(height * width * sizeof(int));

    for (i=0; i < height; i++) {
        for (j=0; j < width; j++) {
            cr = j * dx - 2;
            ci = i * dy - 1;
            image[i*width + j] = insideMandelbrot(cr, ci, 255);
        }
    }
     
    f = fopen("mandelbrot.ppm", "w");
    fprintf(f, "P2\n");
    fprintf(f, "%d %d\n", width, height);
    fprintf(f, "255\n");
    for (i=0; i < height; i++) {
        for (j=0; j < width; j++) {
            fprintf(f, "%d ", image[i*width + j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    free(image);

    return 0;
}
        
