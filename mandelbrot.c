#include <stdio.h>
#include <stdlib.h>
#include "util.h"


int main(int argc, char** argv)
{

    
    int i, j;

    struct Screen s;
    s = getScreen(argc, argv);
    double cr, ci;
    FILE *f;
    int *image = (int *)malloc(s.screensize * sizeof(int));

    for (i=0; i < s.height; i++) {
        for (j=0; j < s.width; j++) {
            cr = j * s.dx - 2;
            ci = i * s.dy - 1;
            image[i*s.width + j] = insideMandelbrot(cr, ci, s.iterations);
        }
    }
    
    f = fopen("mandelbrot.ppm", "w");
    fprintf(f, "P2\n");
    fprintf(f, "%d %d\n", s.width, s.height);
    fprintf(f, "255\n");
    for (i=0; i < s.height; i++) {
        for (j=0; j < s.width; j++) {
            fprintf(f, "%d ", (int)(255*((double)image[i*s.width + j] / s.iterations)));
        }
        fprintf(f, "\n");
    }
    fclose(f);
    free(image);

    return 0;
}
        
